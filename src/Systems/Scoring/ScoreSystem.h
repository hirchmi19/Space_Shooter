//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include <array>
#include <sys/types.h>

#include "../IGameSystem.h"
#include "Entities/Enemies/EnemyType.h"
#include "Locators/IScoreLocator.h"
#include "Utilities/utils.h"

class ScoreSystem: public IScoreLocator, public IGameSystem {

    public:
    ScoreSystem();
    ~ScoreSystem() override = default;

    void Run() override;

    int GetHighScore() const override {return highScore;};
    void AddHighScore(int score) override;
    int GetEnemyScore(const EnemyType& id) override;
    void ResetScore() override {highScore = 0;};


    private:
    int highScore = 0;
    std::array<uint, ToIndex(EnemyType::COUNT)> enemyScores{};

};


