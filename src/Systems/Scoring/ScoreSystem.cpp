//
// Created by Michael Hirsch on 23.02.26.
//

#include "ScoreSystem.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM"){}


void ScoreSystem::Run(GameWorld &world) {

    //NOTHING
}

void ScoreSystem::AddHighScore(const uint32_t score) {

    const uint32_t newScore = highScore + score;
    highScore = newScore >= MAX_SCORE ? MAX_SCORE : newScore;
}

const uint32_t ScoreSystem::GetEnemyScore(const EnemyID& id) {

    switch (id) {

        case EnemyID::YELLOW_ENEMY:
            return YELLOW_ENEMY_SCORE;

        case EnemyID::RED_ENEMY:
            return RED_ENEMY_SCORE;

        case EnemyID::BLACK_ENEMY:
            return BLACK_ENEMY_SCORE;

        default:
            return 0;
    }
}
