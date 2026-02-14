//
// Created by Michael Hirsch on 11.02.26.
//

#include "WaveSystem.h"

#include <iostream>
#include <ostream>

#include "../../Constants/GameConstants.h"
#include "../../Entities/EnemyID.h"
#include "../../Game/GameWorld.h"


WaveSystem::WaveSystem() : IGameSystem(GameSystemID::WAVE_SYSTEM) {

   Init();
}

void WaveSystem::Run(GameWorld& world)
{

    if (waveCompleted) currentPhase = WavePhase::INITIALIZE;

    switch (currentPhase)
    {
        case WavePhase::INITIALIZE:
            StartWave();
            break;

        case WavePhase::FORMATION_OFF:

            HandleDiving(world);
            break;

        case WavePhase::FORMATION_ON:
            // later: attack logic
            break;
    }
}

void WaveSystem::BlockSlot(FormationSlot& slot) const {slot.occupied = true;}

void WaveSystem::CompleteDive() {diveCompleted = true;}

//--------------------------------------------------------------------------

/**
 * Initialize and Calculate all relevant and static gameplay data
 * \return
 */
void WaveSystem::Init() {

    formationPositions.reserve(44);
    formationSlots.reserve(44);
    topDiveSpawns.reserve(44);
    sideDiveSpawns.reserve(44);

    CalcFormationPositions();
    CalcTopDiveSpawns();
    CalcSideDiveSpawns();
    DefinePatterns();
}

void WaveSystem::InitWave() {

    currentPattern = patterns[0]; // later random
    BuildDivingGroups(currentPattern);
}

void WaveSystem::StartWave() {

    waveCounter++;
    waveCompleted = false;
    InitWave();
    currentPhase = WavePhase::FORMATION_OFF;
}

void WaveSystem::CalcFormationPositions()
{
    size_t slot = 0;

    for (size_t row = 0; row < ROW_COUNTS.size(); ++row)
    {
        const int count = ROW_COUNTS[row];

        const float rowWidth = (count - 1) * HORIZONTAL_SPACING;
        const float startX = (GameConstants::SCREEN_WIDTH - rowWidth) / 2.0f;

        for (int i = 0; i < count; ++i)
        {
            constexpr float startY = 100.0f;

            formationPositions[slot].x = startX + i * HORIZONTAL_SPACING;
            formationPositions[slot].y = startY + row * VERTICAL_SPACING;

            slot++;
        }
    }
}


void WaveSystem::CalcTopDiveSpawns() {

    constexpr float leftX  = GameConstants::SCREEN_WIDTH * 0.25f;
    constexpr float rightX = GameConstants::SCREEN_WIDTH * 0.75f;

    constexpr int total = 25;

    for (int i = 0; i < total; ++i)
    {
        Vector2 spawn;

        spawn.x = (i % 2 == 0) ? leftX : rightX;
        spawn.y = -static_cast<float>(i / 2) * VERTICAL_SPACING;

       topDiveSpawns[i] = spawn;
    }
}

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

        spawn.y = rowSpacingY + row * 80.0f + (indexInRow / 2) * verticalSpacing;

        sideDiveSpawns[i] = spawn;
    }
}

void WaveSystem::InitTopDive(GameWorld& world) {

    const auto indices = GetGroupMemberIndices(diveCount);

    for (size_t i = 0; i < indices.size(); ++i) {

        const auto& slot = formationSlots[indices[i]];

        const Vector2 spawn = topDiveSpawns[i];

        world.SpawnEnemy(world.GetEnemySprites(slot.id), spawn);

        Enemy& enemy = world.GetEnemies().back();
        enemy.wave.formationPosition = slot.position;
        enemy.wave.diveGroup = diveCount;
        enemy.wave.state = WaveState::DIVING;

        DefineBezierTop(enemy, spawn, slot);
    }
}


void WaveSystem::DefinePatterns() {

    patterns[0]= {WaveType::TOP_SIDE_TOP, 3, {16, 16, 12},
    {DiveType::FROM_TOP, DiveType::FROM_SIDES, DiveType::FROM_TOP}
    };

    patterns[1] = {
        WaveType::SIDE_TOP_SIDE, 3, {16, 16, 12},
{DiveType::FROM_SIDES, DiveType::FROM_TOP, DiveType::FROM_SIDES}
    };
}

void WaveSystem::DefineBezierTop(Enemy& enemy, const Vector2& spawn, const FormationSlot& slot) {

    enemy.wave.controlPoints[0] = spawn;

    enemy.wave.controlPoints[1] = {spawn.x * 0.8f + slot.position.x * 0.2f,spawn.y + 400.0f };
    enemy.wave.controlPoints[2] = {slot.position.x,spawn.y + 600.0f };

    enemy.wave.controlPoints[3] = slot.position;

    enemy.wave.t = 0.0f;
}

void WaveSystem::BuildDivingGroups(const WavePattern& pattern)
{
    formationSlots.clear();

    size_t index = 0;

    for (int i = 0; i < pattern.numberOfDives; ++i)
    {
        for (int j = 0; j < pattern.groupSizes[i]; ++j)
        {
            FormationSlot slot;

            slot.position = formationPositions[index];
            slot.group.groupID = i;
            slot.group.pattern = pattern.dives[i];
            slot.occupied = false;

            // Assign enemy type depending on slot
            if (index < 4)slot.id = EnemyID::BLACK_ENEMY;
            else if (index < 20)slot.id = EnemyID::RED_ENEMY;
            else slot.id = EnemyID::YELLOW_ENEMY;

            formationSlots.push_back(slot);
            index++;
        }
    }
}

std::vector<size_t> WaveSystem::GetGroupMemberIndices(const int id) const {

    std::vector<size_t> result;

    for (size_t i = 0; i < formationSlots.size(); ++i) {

        if (formationSlots[i].group.groupID == id) result.push_back(i);
    }


    return result;
}

void WaveSystem::HandleDiving(GameWorld& world)
{
    if (!diveTimer.IsFinished())
        return;

    if (diveCount >= currentPattern.numberOfDives)
    {
        currentPhase = WavePhase::FORMATION_ON;
        return;
    }

    if (!diveSpawned)
    {
        const DiveType type = currentPattern.dives[diveCount];

        switch (type)
        {
            case DiveType::FROM_TOP:
                InitTopDive(world);
                break;

            case DiveType::FROM_SIDES:
                // InitSideDive(world);
                break;
        }

        diveSpawned = true;
        diveCompleted = false;
        return;
    }


    if (IsCurrentDiveFinished(world))
    {
        diveSpawned = false;
        diveCompleted = true;
        diveCount++;
        diveTimer.Start(1.5);
    }
}


bool WaveSystem::IsCurrentDiveFinished(GameWorld& world) const
{
    const auto& enemies = world.GetEnemies();

    for (const auto& enemy : enemies)
    {
        if (!enemy.combat.IsAlive())
            continue;

        if (enemy.wave.diveGroup != diveCount)
            continue;

        if (enemy.wave.t < 1.0)
            return false;
    }

    return true;
}






