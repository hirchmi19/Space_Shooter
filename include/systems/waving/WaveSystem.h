//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once

#include <vector>
#include <array>

#include "Wave.h"
#include "../../game/IGameSystem.h"
#include "../../utils//utils.h"
#include "../../locators/IWaveLocator.h"


class WaveSystem : public IWaveLocator , public IGameSystem{


    public:
    WaveSystem();
    ~WaveSystem() override = default;

    void Run() override;
    void Init() override;
    void InitTimers() override;

    void Start();
    void ResetWaveCounter() {waveCounter = 1 ;}
    int GetWaveCounter() const override {return waveCounter;}
    bool IsWaveInitialized() const { return waveInitialized; }
    void SetWaveFinished() { waveCounter++; waveFinished = true; }

    bool waveFinished = false;

    private:

    bool waveInitialized = false;
    bool diveSpawned = false;
    bool diveCompleted = true;
    bool enemiesSpawned = false;

    int diveCount = 0;
    int shootsFired = 0;
    int waveCounter = 1;

    size_t phaseTimer;
    size_t diveTimer;
    size_t attackTimer;

    std::vector<Vector2> formationPositions{};
    std::vector<FormationSlot> formationSlots{};

    std::vector<Vector2> topDiveSpawns{};
    std::vector<Vector2> sideDiveSpawns{};

    WavePhase currentPhase = WavePhase::INITIALIZE;
    WavePattern currentPattern{};

    std::array<WavePattern, ToIndex(WaveType::COUNT)> patterns{};


    void StartWave();

    void DefinePatterns();
    const WavePattern& PickWavePattern() const;

    void AssignBezierTop(Enemy& enemy, const Vector2& start, const Vector2& end);
    void AssignBezierSide(Enemy& enemy, const Vector2& start, const Vector2& end);
    void AssignBezierSolo(Enemy& enemy, const Vector2& start, const Vector2& end);

    void BuildDivingGroups(const WavePattern &pattern);
    void BuildFormationSlots();

    std::vector<size_t> GetGroupMemberIndices(int id) const;

    void SpawnDive(DiveType type);

    void AssignDiveCurves(DiveType type);
    void AssignAttackCurves(DiveType type);
    void AssignCurve(Enemy& enemy,const Vector2& start, const Vector2& end, DiveType type);

    void BuildFormation();
    void HandleSoloAttacks();
    void HandleSwarmAttacks();
    void HandleFormationAttacks();

    bool IsCurrentDiveFinished() const;

    void CalcFormationPositions();
    void CalcTopDiveSpawns();
    void CalcSideDiveSpawns();

};


