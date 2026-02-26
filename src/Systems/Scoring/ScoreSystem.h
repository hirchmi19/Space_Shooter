//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include "../IGameSystem.h"
#include "../../Entities/EnemyID.h"


inline constexpr uint32_t MAX_SCORE = 99999999;
inline constexpr uint32_t YELLOW_ENEMY_SCORE = 300;
inline constexpr uint32_t RED_ENEMY_SCORE = 500;
inline constexpr uint32_t BLACK_ENEMY_SCORE = 1000;

class ScoreSystem: public IGameSystem{

    public:
    ScoreSystem();
    ~ScoreSystem() override = default;

    void Run(GameWorld &world) override;
    const uint32_t GetHighScore() const {return highScore;};
    void AddHighScore(uint32_t score);
    const uint32_t GetEnemyScore(const EnemyID& id);
    void ResetScore() {highScore = 0;};


    private:
    uint32_t highScore = 0;

};


