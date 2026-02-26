//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include "../IGameSystem.h"
#include "../../Entities/Enemies/EnemyID.h"

class ScoreSystem: public IGameSystem{

    public:
    ScoreSystem();
    ~ScoreSystem() override = default;

    const uint32_t GetHighScore() const {return highScore;};
    void AddHighScore(uint32_t score);
    const uint32_t GetEnemyScore(const EnemyID& id);
    void ResetScore() {highScore = 0;};


    private:
    uint32_t highScore = 0;

};


