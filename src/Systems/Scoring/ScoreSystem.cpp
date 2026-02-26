//
// Created by Michael Hirsch on 23.02.26.
//

#include "ScoreSystem.h"

#include "Constants/ScoringConstants.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM"){}

void ScoreSystem::AddHighScore(const uint32_t score) {

    const uint32_t newScore = highScore + score;
    highScore = newScore >= ScoringConstants::MAX_SCORE ? ScoringConstants::MAX_SCORE : newScore;
}

const uint32_t ScoreSystem::GetEnemyScore(const EnemyID& id) {

    switch (id) {

        case EnemyID::YELLOW_ENEMY:
            return ScoringConstants::YELLOW_ENEMY_SCORE;

        case EnemyID::RED_ENEMY:
            return ScoringConstants::RED_ENEMY_SCORE;

        case EnemyID::BLACK_ENEMY:
            return ScoringConstants::BLACK_ENEMY_SCORE;

        default:
            return 0;
    }
}
