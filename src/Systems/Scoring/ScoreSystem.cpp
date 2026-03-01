//
// Created by Michael Hirsch on 23.02.26.
//

#include "ScoreSystem.h"

#include "Constants/ScoringConstants.h"
#include "Entities/Enemies/EnemyType.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM"){}


void ScoreSystem::AddHighScore(const uint32_t score) {

    const uint32_t newScore = highScore + score;
    highScore = newScore >= ScoringConstants::MAX_SCORE ? ScoringConstants::MAX_SCORE : newScore;
}

 uint32_t ScoreSystem::GetEnemyScore(const EnemyType& id) {

    switch (id) {

        case EnemyType::YELLOW_E:
            return ScoringConstants::YELLOW_ENEMY_SCORE;

        case EnemyType::RED_E:
            return ScoringConstants::RED_ENEMY_SCORE;

        case EnemyType::BLACK_E:
            return ScoringConstants::BLACK_ENEMY_SCORE;

        default:
            return 0;
    }
}
