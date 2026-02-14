//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once

#include <vector>
#include <array>

#include "FormationSlot.h"
#include "WavePattern.h"
#include "WavePhase.h"
#include "WaveType.h"
#include "../IGameSystem.h"
#include "../../Entities/Components/TimerComponent.h"
#include "../../Utilities/utils.h"


constexpr std::array ROW_COUNTS = {4, 8, 8, 12, 12};
constexpr float HORIZONTAL_SPACING = 60.0f;
constexpr float VERTICAL_SPACING   = 65.0f;
static uint32_t waveCounter = 0;


class WaveSystem : public IGameSystem{


    public:
    WaveSystem();
    ~WaveSystem() override = default;

    void Run(GameWorld &world) override;

    private:

    bool waveCompleted = true;
    bool diveSpawned = false;
    bool diveCompleted = true;
    int diveCount = 0;

    TimerComponent phaseTimer;
    TimerComponent diveTimer;

    std::vector<Vector2> formationPositions{};

    std::vector<FormationSlot> formationSlots{};

    std::vector<Vector2> topDiveSpawns{};
    std::vector<Vector2> sideDiveSpawns{};

    WavePhase currentPhase = WavePhase::NONE;
    WavePattern currentPattern;

    std::array<WavePattern, ToIndex(WaveType::COUNT)> patterns{};

    void Init();
    void StartWave(GameWorld& world);

    void DefinePatterns();
    //void PickWavePattern();

    void DefineBezierTop(Enemy& enemy, const Vector2& spawn, const Vector2& slotPosition);
    void DefineBezierSide(Enemy& enemy, const Vector2& spawn, const Vector2& slotPosition);

    void BuildDivingGroups(const WavePattern &pattern);
    std::vector<size_t> GetGroupMemberIndices(int id) const;

    void SpawnDive(GameWorld& world, const DiveType type);
    void AssignDiveCurves(GameWorld& world, DiveType type);

    void HandleDiving(GameWorld& world);
    bool IsCurrentDiveFinished(GameWorld& world) const;

    void CalcFormationPositions();
    void CalcTopDiveSpawns();
    void CalcSideDiveSpawns();

};


