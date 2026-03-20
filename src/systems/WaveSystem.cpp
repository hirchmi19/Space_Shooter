//
// Created by Michael Hirsch on 11.02.26.
//

#include <iostream>
#include <ostream>
#include "systems/waving/Wave.h"
#include "systems/waving/WaveSystem.h"
#include "constants/GameConstants.h"
#include "constants/TimerDurations.h"
#include "constants/WaveConstants.h"
#include "locators/SystemLocator.h"
#include "systems/waving/BezierCurves.h"


WaveSystem::WaveSystem() : IGameSystem(GameSystemID::WAVE_SYSTEM, "WAVE_SYSTEM") {}

void WaveSystem::Run()
{

   // if (phaseTimer.IsRunning()) return;

    switch (currentPhase)
    {
        case WavePhase::INITIALIZE:

            StartWave();
            break;

        case WavePhase::FORMATION_OFF:

            BuildFormation();
            break;

        case WavePhase::FORMATION_ON:

            HandleSoloAttacks();
            HandleFormationAttacks();
            break;

        case WavePhase::READY_FOR_SWARM:

            HandleSwarmAttacks();
            break;
    }

    if (SystemLocator::entityLocator->GetEnemies().empty() && waveInitialized) {

        waveCounter++;
        waveFinished = true;
    }

}

void WaveSystem::Start() {

    currentPhase = WavePhase::INITIALIZE;
    waveFinished = false;

}


//--------------------------------------------------------------------------

/**
 * Initialize and Calculate all relevant and static wave data
 * \return
 */
void WaveSystem::Init() {

    formationPositions.resize(WaveConstants::NUMBER_OF_ENEMIES);
    formationSlots.resize(WaveConstants::NUMBER_OF_ENEMIES);
    topDiveSpawns.resize(WaveConstants::NUMBER_OF_ENEMIES);
    sideDiveSpawns.resize(WaveConstants::NUMBER_OF_ENEMIES);

    InitTimers();

    CalcFormationPositions();
    CalcTopDiveSpawns();
    CalcSideDiveSpawns();
}

void WaveSystem::InitTimers() {

    diveTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);
    attackTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);
    phaseTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);

}

/**
 * Inits a new Wave and starts the state machine
 */
void WaveSystem::StartWave() {

    waveInitialized = false;

    divingGroupCounter = GetRandomValue(WaveConstants::MIN_DIVES, WaveConstants::MAX_DIVES);
    diveCount = 0;
    diveSpawned = false;
    diveCompleted = false;
    enemiesSpawned = false;
    shootsFired = 0;

    BuildFormationSlots();
    BuildDivingGroups();
    currentPhase = WavePhase::FORMATION_OFF;
}


/**
 * Handles diving when all enemies are outside the screen
 */
void WaveSystem::BuildFormation()
{
    if (SystemLocator::timerLocator->IsRunning(diveTimer) && diveSpawned) return;

    if (diveCount >= divingGroupCounter)
    {
        currentPhase = WavePhase::FORMATION_ON;
        diveCount = 0;
        enemiesSpawned = true;
        waveInitialized = true;
        SystemLocator::timerLocator->Start(TimerDurations::WAVE_PHASE, phaseTimer);
        return;
    }

    if (!diveSpawned)
    {

        const DiveType& type = ToEnum<DiveType>(static_cast<size_t>(GetRandomValue(1, ToIndex(DiveType::COUNT) - 1 )));
        if (!enemiesSpawned) SpawnDive(type);
        AssignDiveCurves(type);

        diveSpawned = true;
        diveCompleted = false;
        return;
    }

    if (IsCurrentDiveFinished() && diveSpawned)
    {
        diveSpawned = false;
        diveCompleted = true;
        diveCount++;
        SystemLocator::timerLocator->Start(TimerDurations::WAVE_DIVE, diveTimer);
    }
}

/**
 * Random number of enemies swarm out and attack player directly
 */
void WaveSystem::HandleSoloAttacks() {

    if (SystemLocator::timerLocator->IsRunning(diveTimer) ||
        SystemLocator::timerLocator->IsRunning(phaseTimer)) return;

    auto& enemies = SystemLocator::entityLocator->GetEnemies();

    if (enemies.empty()) return;

    if (enemies.size() < 4) {

        currentPhase = WavePhase::READY_FOR_SWARM;
        diveCount = divingGroupCounter;
        shootsFired = 0;
        return;
    }

    const int maxEnemies = std::max(4, GetRandomValue(4, static_cast<int>(enemies.size() * 0.25f))); // at least 4 up to 1/4 of the total enemies can solo attack
    int enemyIndex = 0;

    if (diveCount >= maxEnemies) {

        currentPhase = WavePhase::READY_FOR_SWARM;
        diveCount = divingGroupCounter;
        shootsFired = 0;
        return;
    }

    do {
        enemyIndex = GetRandomValue(0, static_cast<int>(enemies.size() - 1));
    }while (enemies[enemyIndex].wave.state != WaveState::IN_FORMATION);

    CalcControlPointsSolo(enemies[enemyIndex],
        enemies[enemyIndex].wave.formationPosition,
        SystemLocator::entityLocator->GetPlayer()->GetPosition());

    enemies[enemyIndex].wave.state = WaveState::ATTACK;
    diveCount++;
    SystemLocator::timerLocator->Start(TimerDurations::WAVE_DIVE, diveTimer);
}

/**
 * Remaining enemies swarm out and attack player directly
 */
void WaveSystem::HandleSwarmAttacks() {


    if (SystemLocator::timerLocator->IsRunning(diveTimer) && diveSpawned ||
        SystemLocator::entityLocator->GetEnemies().empty()) return;

    // phase shift
    if (diveCount  < 0) {

        currentPhase = WavePhase::FORMATION_OFF;
        diveCount = 0;
        SystemLocator::timerLocator->Start(TimerDurations::WAVE_PHASE, phaseTimer);
        return;
    }

    // spawn and execute next dive
    if (!diveSpawned) {

        AssignAttackCurves();

        diveSpawned = true;
        diveCompleted = false;
        return;
    }

    // wait for dive to finish
    if (IsCurrentDiveFinished() && diveSpawned)
    {
        diveSpawned = false;
        diveCompleted = true;
        diveCount--;
        SystemLocator::timerLocator->Start(TimerDurations::WAVE_DIVE, diveTimer);
    }

}

/**
 * Random number of enemies inside the formation shoot
 */
void WaveSystem::HandleFormationAttacks() {

    const auto& enemies = SystemLocator::entityLocator->GetEnemies();

    if (enemies.size() <= 2) return; // not enough enemies left in the formation

    const int maxShots = static_cast<int>(SystemLocator::entityLocator->GetEnemies().size() / 2);

    if (shootsFired >= maxShots || SystemLocator::timerLocator->IsRunning(attackTimer)) return;

    int enemyIndex = 0;

    do {
        enemyIndex = GetRandomValue(0, static_cast<int>(enemies.size() - 1));
    }while (enemies[enemyIndex].wave.state != WaveState::IN_FORMATION);

    SystemLocator::entityLocator->SpawnProjectile(
        ProjectileType::BASE_ENEMY,
        enemies[enemyIndex].position,
        false);

    SystemLocator::timerLocator->Start(TimerDurations::WAVE_ATTACK, attackTimer);
    shootsFired++;
}

/**
 * Define formation slots and assign enemies randomly
 */
void WaveSystem::BuildFormationSlots() {

    formationSlots.clear();

    constexpr EnemyType topEnemy = EnemyType::BLACK_E; // black enemies are always in the top row

    const EnemyType middleEnemy = ToEnum<EnemyType>(   // possible enemies for middel rows of formation
            static_cast<size_t>(GetRandomValue(
                    ToIndex(EnemyType::RED_E),
                    ToIndex(EnemyType::BLUE_E)
                    )));

    EnemyType bottomEnemy = EnemyType::NONE;

    if (middleEnemy == EnemyType::RED_E) bottomEnemy = EnemyType::YELLOW_E; // possible enemies for bottoms rows of formation
    else {
        bottomEnemy = ToEnum<EnemyType>(
         static_cast<size_t>(GetRandomValue(
                 ToIndex(EnemyType::YELLOW_E),
                 ToIndex(EnemyType::RED_E)
                 )));
    }

    for (size_t i = 0; i < formationPositions.size(); ++i) {

        FormationSlot slot;
        slot.position = formationPositions[i];

        if (i < WaveConstants::TOP_ROW_ENEMIES)
            slot.type = topEnemy;
        else if (i < WaveConstants::MIDDLE_ROW_ENEMIES)
            slot.type = middleEnemy;
        else
            slot.type = bottomEnemy;

        formationSlots.push_back(slot);
        }
    }

/**
 * Partitions enemies in diving groups or sub-waves
 */
void WaveSystem::BuildDivingGroups()
{
    size_t index = 0;
    const int enemiesPerGroup = WaveConstants::NUMBER_OF_ENEMIES / divingGroupCounter;

    // assign all enemies to their diving groups except for the last group
    for (int dive = 0; dive < divingGroupCounter - 1; ++dive)
    {
        for (int slot = 0; slot < enemiesPerGroup; ++slot)
        {
            formationSlots[index].groupID = dive;
            index++;
        }
    }

    // fill last diving group with remaining enemies
    while (index < formationSlots.size()) {

        formationSlots[index].groupID = divingGroupCounter - 1;
        index++;
    }
}

/**
 * Spawns all members of a diving group at the correct location
 * \param type
 */
void WaveSystem::SpawnDive(const DiveType type) {

    const auto indices = GetGroupMemberIndices(diveCount);
    const auto& spawns = (type == DiveType::TOP) ? topDiveSpawns : sideDiveSpawns;

    for (size_t i = 0; i < indices.size(); ++i)
    {
        const auto& slot = formationSlots[indices[i]];
        const Vector2 spawn = spawns[i];

        SystemLocator::entityLocator->SpawnEnemy(slot.type, spawn);

        Enemy& enemy = SystemLocator::entityLocator->GetEnemies().back();
        enemy.wave.formationPosition = slot.position;
        enemy.wave.diveGroup = diveCount;
        enemy.wave.spawnPosition = spawn;
        enemy.wave.slotIndex = slot.gridIndex;
    }
}

/**
 * Assigns all members of a diving group to the correct Bézier curve
 */
void WaveSystem::AssignDiveCurves(const DiveType &type)
{
    auto& enemies = SystemLocator::entityLocator->GetEnemies();

    for (auto& enemy : enemies)
    {
        if (enemy.wave.diveGroup != diveCount)
            continue;

        AssignCurve(enemy, enemy.wave.spawnPosition, enemy.wave.formationPosition, type);
    }
}

void WaveSystem::AssignAttackCurves() {

    auto& enemies = SystemLocator::entityLocator->GetEnemies();

    for (auto& enemy : enemies) {

        const Vector2& end = {enemy.wave.formationPosition.x,SystemLocator::entityLocator->GetPlayer()->GetPosition().y + 100.f};

        if (enemy.wave.diveGroup != diveCount) continue;
        if (enemy.wave.state != WaveState::IN_FORMATION) continue;

        CalcControlPointsSide(enemy, enemy.wave.formationPosition, end);
        enemy.wave.state = WaveState::ATTACK;
    }
}

void WaveSystem::AssignCurve(Enemy &enemy, const Vector2& start, const Vector2 &end, const DiveType type) {

    switch (type) {

        case DiveType::TOP:
            CalcControlPointsTop(enemy, start, end);
            break;

        case DiveType::SIDE:
            CalcControlPointsSide(enemy, start, end);
            break;

        case DiveType::SIDE_RANDOM:
            CalcControlPointsSideRandom(enemy, start, end);
            break;

        case DiveType::LOOP:
            CalcControlPointsLoop(enemy, start, end);
            break;

        case DiveType::LOOP_RANDOM:
            CalcControlPointsLoopRandom(enemy, start, end);
            break;
    }

    enemy.wave.state = WaveState::ENTER_FORMATION;

}

/**
 * Checks if a dive is completed
 * \return
 */
bool WaveSystem::IsCurrentDiveFinished() const
{
    const auto& enemies = SystemLocator::entityLocator->GetEnemies();

    for (const auto& enemy : enemies)
    {
        if (!enemy.isAlive)
            continue;

        if (enemy.wave.diveGroup != diveCount)
            continue;

        if (enemy.wave.t < 1.0)
            return false;
    }

    return true;
}

/**
 * Returns the indices of all group members of a diving group
 * \param id
 * \return
 */
std::vector<size_t> WaveSystem::GetGroupMemberIndices(const int id) const {

    std::vector<size_t> result;

    for (size_t i = 0; i < formationSlots.size(); ++i) {

        if (formationSlots[i].groupID == id) result.push_back(i);
    }

    return result;
}


//PRE-COMPUTATIONS --------------------------------------------------------------------------


/**
 * Computes the formation layout
 */
void WaveSystem::CalcFormationPositions()
{
    size_t slot = 0;

    for (size_t row = 0; row < WaveConstants::ROW_COUNTS.size(); ++row)
    {
        const int count = WaveConstants::ROW_COUNTS[row];

        const float rowWidth = static_cast<float>(count - 1) * WaveConstants::HORIZONTAL_SPACING;
        const float startX = (GameConstants::SCREEN_WIDTH - rowWidth) / 2.0f;

        for (int i = 0; i < count; ++i)
        {
            constexpr float startY = 100.0f;

            formationPositions[slot].x = startX + static_cast<float>(i) * WaveConstants::HORIZONTAL_SPACING;
            formationPositions[slot].y = startY + static_cast<float>(row) * WaveConstants::VERTICAL_SPACING;

            slot++;
        }
    }
}


/**
 * Computes the spawn positions for the top dive
 */
void WaveSystem::CalcTopDiveSpawns() {

    constexpr float leftX  = GameConstants::SCREEN_WIDTH * 0.25f;
    constexpr float rightX = GameConstants::SCREEN_WIDTH * 0.75f;

    constexpr int total = 25;

    for (int i = 0; i < total; ++i)
    {
        Vector2 spawn;

        spawn.x = (i % 2 == 0) ? leftX : rightX;
        spawn.y = -static_cast<float>(i) / 2 * WaveConstants::VERTICAL_SPACING - 50.f;

        topDiveSpawns[i] = spawn;
    }
}

/**
 * Computes the spawn positions for the side dive
 */
void WaveSystem::CalcSideDiveSpawns()
{
    constexpr float horizontalOffset = 60.0f;
    constexpr float rowSpacingY      = 120.0f;
    constexpr float verticalSpacing  = 50.0f;

    constexpr int total = 25;
    constexpr int perRow = total / 2;

    for (int i = 0; i < total; ++i)
    {
        Vector2 spawn{};

        const int row = i / perRow;
        const int indexInRow = i % perRow;

        const bool fromLeft = (indexInRow % 2 == 0);

        spawn.x = fromLeft? -horizontalOffset : GameConstants::SCREEN_WIDTH + horizontalOffset;

        spawn.y = rowSpacingY + static_cast<float>(row) * 80.0f + static_cast<float>(indexInRow) / 2 * verticalSpacing;

        sideDiveSpawns[i] = spawn;
    }
}

