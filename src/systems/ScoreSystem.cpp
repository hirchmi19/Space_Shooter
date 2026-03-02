//
// Created by Michael Hirsch on 23.02.26.
//

#include "systems/scoring/ScoreSystem.h"
#include "constants/ScoringConstants.h"
#include "entities/enemies/EnemyType.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM"){}


void ScoreSystem::Run() {


}

void ScoreSystem::AddHighScore(const int score) {

    const int newScore = highScore + score;
    highScore = newScore >= ScoringConstants::MAX_SCORE ? ScoringConstants::MAX_SCORE : newScore;
}

 int ScoreSystem::GetEnemyScore(const EnemyType& id) {

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
