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

class WaveSystem : public IGameSystem{


    public:
    WaveSystem();
    ~WaveSystem() override = default;

    static int WaveCounter;

    void Run(GameWorld &world) override;
    void Start();
    void ResetWaveCounter() {waveCounter = 1 ;}
    int GetWaveCounter() const {return waveCounter;};

    private:

    bool waveInitialized = false;
    bool diveSpawned = false;
    bool diveCompleted = true;
    bool enemiesSpawned = false;

    int diveCount = 0;
    int shootsFired = 0;
    int waveCounter = 1;

    TimerComponent phaseTimer;
    TimerComponent diveTimer;
    TimerComponent attackTimer;

    std::vector<Vector2> formationPositions{}; //TODO: change to arrays
    std::vector<FormationSlot> formationSlots{};

    std::vector<Vector2> topDiveSpawns{};
    std::vector<Vector2> sideDiveSpawns{};

    WavePhase currentPhase = WavePhase::INITIALIZE;
    WavePattern currentPattern;

    std::array<WavePattern, ToIndex(WaveType::COUNT)> patterns{};

    void Init();
    void StartWave();

    void DefinePatterns();
    WavePattern PickWavePattern() const;

    void AssignBezierTop(Enemy& enemy, const Vector2& start, const Vector2& end);
    void AssignBezierSide(Enemy& enemy, const Vector2& start, const Vector2& end);
    void AssignBezierSolo(Enemy& enemy, const Vector2& start, const Vector2& end);

    void BuildDivingGroups(const WavePattern &pattern);
    void BuildFormationSlots();

    std::vector<size_t> GetGroupMemberIndices(int id) const;

    void SpawnDive(GameWorld& world, const DiveType type);

    void AssignDiveCurves(GameWorld& world, DiveType type);
    void AssignAttackCurves(GameWorld& world, DiveType type);
    void AssignCurve(Enemy& enemy,const Vector2& start, const Vector2& end, DiveType type);

    void BuildFormation(GameWorld& world);
    void HandleSoloAttacks(GameWorld& world);
    void HandleSwarmAttacks(GameWorld& world);
    void HandleFormationAttacks(GameWorld& world);

    bool IsCurrentDiveFinished(GameWorld& world) const;

    void CalcFormationPositions();
    void CalcTopDiveSpawns();
    void CalcSideDiveSpawns();

};


