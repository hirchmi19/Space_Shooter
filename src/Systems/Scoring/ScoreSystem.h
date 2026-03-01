//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include "../IGameSystem.h"
#include "../../Entities/EntityType.h"
#include "Locators/IScoreLocator.h"

class ScoreSystem: public IScoreLocator, public IGameSystem {

    public:
    ScoreSystem();
    ~ScoreSystem() override = default;

    uint32_t GetHighScore() const {return highScore;};
    void AddHighScore(uint32_t score) override;
    uint32_t GetEnemyScore(const EnemyType& id) override;
    void ResetScore() override {highScore = 0;};


    private:
    uint32_t highScore = 0;

};


