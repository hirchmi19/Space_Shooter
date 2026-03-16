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
    BuildFormationSlots();
    DefinePatterns();
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

    diveCount = 0;
    diveSpawned = false;
    diveCompleted = false;
    enemiesSpawned = false;
    shootsFired = 0;
    currentPattern = PickWavePattern();

    BuildDivingGroups(currentPattern);
    currentPhase = WavePhase::FORMATION_OFF;
}


/**
 * Handles diving when all enemies are outside the screen
 */
void WaveSystem::BuildFormation()
{
    if (SystemLocator::timerLocator->IsRunning(diveTimer) && diveSpawned) return;

    if (diveCount >= currentPattern.numberOfDives)
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
        const DiveType type = currentPattern.dives[diveCount];

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
        diveCount = currentPattern.numberOfDives;
        shootsFired = 0;
        return;
    }

    const int maxEnemies = std::max(4, GetRandomValue(4, static_cast<int>(enemies.size() * .25f))); // at least 4 up to 1/4 of the total enemies can solo attack
    int enemyIndex = 0;

    if (diveCount >= maxEnemies) {

        currentPhase = WavePhase::READY_FOR_SWARM;
        diveCount = currentPattern.numberOfDives;
        shootsFired = 0;
        return;
    }

    do {
        enemyIndex = GetRandomValue(0, static_cast<int>(enemies.size() - 1));
    }while (enemies[enemyIndex].wave.state != WaveState::IN_FORMATION);

    AssignBezierSolo(enemies[enemyIndex],
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

    if (diveCount  < 0) {

        currentPhase = WavePhase::FORMATION_OFF;
        diveCount = 0;
        SystemLocator::timerLocator->Start(TimerDurations::WAVE_PHASE, phaseTimer);
        return;
    }

    if (!diveSpawned) {

        const DiveType type = currentPattern.dives[diveCount];

        AssignAttackCurves(type);

        diveSpawned = true;
        diveCompleted = false;
        return;
    }

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

    if (enemies.size() <= 2) return;

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

void WaveSystem::BuildFormationSlots() {

    formationSlots.clear();

    for (size_t i = 0; i < formationPositions.size(); ++i)
    {
        FormationSlot slot;
        slot.position = formationPositions[i];

        if (i < 4) // 4 black enemies, and 20 red and yellow enemies (= 44)
            slot.type = EnemyType::BLACK_E;
        else if (i < 20)
            slot.type = EnemyType::RED_E;
        else
            slot.type = EnemyType::YELLOW_E;

        formationSlots.push_back(slot);
    }

}

/**
 * Separates enemies in diving groups or sub-waves
 * \param pattern
 */
void WaveSystem::BuildDivingGroups(const WavePattern& pattern)
{
    size_t index = 0;

    for (int dive = 0; dive < pattern.numberOfDives; ++dive)
    {
        for (int j = 0; j < pattern.groupSizes[dive]; ++j)
        {
            formationSlots[index].group.groupID = dive;
            formationSlots[index].group.dive = pattern.dives[dive];
            index++;
        }
    }
}

/**
 * Spawns all members of a diving group at the correct location
 * \param type
 */
void WaveSystem::SpawnDive(const DiveType type) {

    const auto indices = GetGroupMemberIndices(diveCount);
    const auto& spawns = (type == DiveType::FROM_TOP) ? topDiveSpawns : sideDiveSpawns;

    for (size_t i = 0; i < indices.size(); ++i)
    {
        const auto& slot = formationSlots[indices[i]];
        const Vector2 spawn = spawns[i];

        SystemLocator::entityLocator->SpawnEnemy(slot.type, spawn);

        Enemy& enemy = SystemLocator::entityLocator->GetEnemies().back();
        enemy.wave.formationPosition = slot.position;
        enemy.wave.diveGroup = diveCount;
        enemy.wave.spawnPosition = spawn;
        enemy.wave.slotIndex = slot.slotIndex;
    }
}

/**
 * Assigns all members of a diving group to the correct Bézier curve
 * \param type
 */
void WaveSystem::AssignDiveCurves(const DiveType type)
{
     auto& enemies = SystemLocator::entityLocator->GetEnemies();

    for (auto& enemy : enemies)
    {
        if (enemy.wave.diveGroup != diveCount)
            continue;

        AssignCurve(enemy, enemy.wave.spawnPosition, enemy.wave.formationPosition, type);
    }
}

void WaveSystem::AssignAttackCurves(const DiveType type) {

    auto& enemies = SystemLocator::entityLocator->GetEnemies();

    for (auto& enemy : enemies) {

        const Vector2& end = {enemy.wave.formationPosition.x, SystemLocator::entityLocator->GetPlayer()->GetPosition().y + 100.f};

        if (enemy.wave.diveGroup != diveCount)continue;
        if (enemy.wave.state != WaveState::IN_FORMATION) continue;

        AssignBezierSide(enemy, enemy.wave.formationPosition, end);
        enemy.wave.state = WaveState::ATTACK;
    }
}

void WaveSystem::AssignCurve(Enemy &enemy, const Vector2& start, const Vector2 &end, const DiveType type) {

    switch (type) {

        case DiveType::FROM_TOP:

            AssignBezierTop(enemy, start, end);
            enemy.wave.state = WaveState::ENTER_FORMATION;
            break;

        case DiveType::FROM_SIDES:

            AssignBezierSide(enemy, start, end);
            enemy.wave.state = WaveState::ENTER_FORMATION;
            break;
    }

}

/**
 * Checks if a dive is completed
 * \return
 */
bool WaveSystem::IsCurrentDiveFinished() const
{
    const auto& enemies = SystemLocator::entityLocator->GetEnemies();

    bool foundAny = false;

    for (const auto& enemy : enemies) {
        if (!enemy.isAlive) continue;
        if (enemy.wave.diveGroup != diveCount) continue;

        foundAny = true;
        if (enemy.wave.t < 1.0f) return false;
    }

    return foundAny; // only true if at least one enemy finished, not if empty
}

/**
 * Returns the indices of all group members of a diving group
 * \param id
 * \return
 */
std::vector<size_t> WaveSystem::GetGroupMemberIndices(const int id) const {

    std::vector<size_t> result;

    for (size_t i = 0; i < formationSlots.size(); ++i) {

        if (formationSlots[i].group.groupID == id) result.push_back(i);
    }

    return result;
}

const WavePattern& WaveSystem::PickWavePattern() const {

    // start at 1 to skip WaveType::NONE at index 0
    const auto patternIndex = static_cast<size_t>(GetRandomValue(1, patterns.size() - 1));
    return patterns[patternIndex];
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


// DEFINITIONS --------------------------------------------------------------------------

/**
 * Define wave patterns
 */
void WaveSystem::DefinePatterns() {

    patterns[ToIndex(WaveType::TOP_SIDE_TOP)]= {WaveType::TOP_SIDE_TOP, 3, {16, 16, 12},
    {DiveType::FROM_TOP, DiveType::FROM_SIDES, DiveType::FROM_TOP}};

    patterns[ToIndex(WaveType::SIDE_TOP_SIDE)] = {WaveType::SIDE_TOP_SIDE, 3, {16, 16, 12},
{DiveType::FROM_SIDES, DiveType::FROM_TOP, DiveType::FROM_SIDES}};

    // Define wave patterns here
}


// Define all Bézier curves here TODO: better to defines this as structs

void WaveSystem::AssignBezierTop(Enemy& enemy, const Vector2& start, const Vector2& end) {

    enemy.wave.controlPoints[0] = start;

    enemy.wave.controlPoints[1] = {start.x * 0.8f + end.x * 0.2f,start.y + 400.0f };
    enemy.wave.controlPoints[2] = {end.x,start.y + 600.0f };

    enemy.wave.controlPoints[3] = end;

    enemy.wave.t = 0.0f;
}

void WaveSystem::AssignBezierSide(Enemy &enemy, const Vector2 &start, const Vector2 &end) {

    enemy.wave.controlPoints[0] = start;

    const bool fromLeft = start.x < 0;

    constexpr float horizontalPull = 200.0f;
    constexpr float verticalPull  = 800.0f;

    enemy.wave.controlPoints[1] = {start.x + (fromLeft ? horizontalPull : -horizontalPull),start.y + verticalPull};
    enemy.wave.controlPoints[2].x = end.x + (fromLeft ? -40.0f : 40.0f);
    enemy.wave.controlPoints[3] = end;

    enemy.wave.t = 0.0f;
}

void WaveSystem::AssignBezierSolo(Enemy &enemy, const Vector2 &start, const Vector2 &end) {

    enemy.wave.controlPoints[0] = start;

    const bool fromLeft = start.x < GameConstants::SCREEN_WIDTH / 2;
    constexpr float horizontalPull = 450.0f;

    enemy.wave.controlPoints[1] = {start.x + (fromLeft ? horizontalPull : -horizontalPull),start.y + 300.0f};
    enemy.wave.controlPoints[2] = {end.x + (fromLeft ? -60.0f : 60.0f),end.y - 150.0f};
    enemy.wave.controlPoints[3] = {end.x, end.y + 100.0f};

    enemy.wave.t = 0.0f;
}