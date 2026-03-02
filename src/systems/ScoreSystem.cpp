//
// Created by Michael Hirsch on 23.02.26.
//


#include <iostream>
#include <ostream>
#include "systems/scoring/ScoreSystem.h"
#include "constants/ScoringConstants.h"
#include "constants/WaveConstants.h"
#include "entities/enemies/EnemyType.h"
#include "locators/SystemLocator.h"
#include "systems/rendering/scoreUI.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM"){}


void ScoreSystem::Run() {

    if (!SystemLocator::timerLocator->IsRunning(multTimer) && timerStarted) {

        const float newMult = mult * 0.33f;
        mult = std::max(newMult, 1.0f); // mult cannot fall under default value
        SystemLocator::timerLocator->Start(3.0f, multTimer); // mult decreases after decrease as well
    }
}\

void ScoreSystem::Init() {

    scoreUI.reserve(WaveConstants::NUMBER_OF_ENEMIES);
    multTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);
}

void ScoreSystem::AddHighScore(const int& score, const Vector2& pos) {

    const float scoreAdd = std::floor(static_cast<float>(score) * mult);
    int newScore = static_cast<int>(scoreAdd + static_cast<float>(highScore));
    newScore = std::min(newScore, ScoringConstants::MAX_SCORE);
    highScore = newScore;
    CreateScore(scoreAdd, pos);
    SystemLocator::timerLocator->Start(2.0f, multTimer); // mult decreases after 2 seconds
    timerStarted = true;

    const float newMult = 0.1f * std::floor(mult) + mult;
    mult = std::min(newMult, ScoringConstants::MAX_MULT);

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


void ScoreSystem::CreateScore(const float &score, const Vector2 &pos) {

    size_t timer = SystemLocator::timerLocator->CreateTimer(0.7f, false);
    scoreUI.emplace_back(score, pos, timer);

}
