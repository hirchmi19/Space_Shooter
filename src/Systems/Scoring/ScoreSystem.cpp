//
// Created by Michael Hirsch on 23.02.26.
//

#include "ScoreSystem.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM){}


void ScoreSystem::Run(GameWorld &world) {

    //NOTHING
}

void ScoreSystem::AddHighScore(const uint32_t score) {

    const uint32_t newScore = highScore + score;

    if (newScore >= MAX_SCORE) {

        highScore = MAX_SCORE;
        return;
    }

    highScore = newScore;
}

const uint32_t ScoreSystem::GetEnemyScore(const EnemyID& id) {

    switch (id) {

        case EnemyID::YELLOW_ENEMY:
            return yellowEnemyScore;

        case EnemyID::RED_ENEMY:
            return redEnemyScore;

        case EnemyID::BLACK_ENEMY:
            return blackEnemyScore;

    }
}
