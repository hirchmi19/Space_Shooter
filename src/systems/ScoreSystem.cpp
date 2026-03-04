//
// Created by Michael Hirsch on 23.02.26.
//

#include "systems/scoring/ScoreSystem.h"
#include "constants/ScoringConstants.h"
#include "constants/WaveConstants.h"
#include "entities/enemies/EnemyType.h"
#include "locators/SystemLocator.h"
#include "systems/rendering/MessageUi.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM") {}


void ScoreSystem::Run() {

  flowStateActive = SystemLocator::entityLocator->GetPlayer()->IsInFlowState();

  if (mult >= 5.0f && !flowStateActive) {
    SystemLocator::entityLocator->GetPlayer()->EnterFlowState();
    flowStateActive = true;
    CreateMessage("ENTERED FLOW STATE!");
  }

  if (!SystemLocator::timerLocator->IsRunning(multTimer) && timerStarted) {

    const float newMult = mult * 0.33f; // decrease mult
    mult = std::max(newMult, 1.0f); // mult cannot fall under default value

    SystemLocator::timerLocator->Start(3.0f, multTimer); // mult decreases after decrease as well
    SystemLocator::entityLocator->GetPlayer()->LeaveFlowState();
    CreateMessage("LEAVED FLOW STATE!");
    ResetMult(); // reset mult back do default, if leaving the flow state
    flowStateActive = false;
  }
}

void ScoreSystem::AddHighScore(const int &score, const Vector2 &pos) {

  flowBonus = flowStateActive ? 2 : 1; // double the points while in flow state

  const float scoreAdd = std::floor(static_cast<float>(score) * mult * flowBonus);
  int newScore = static_cast<int>(scoreAdd + static_cast<float>(highScore));
  newScore = std::min(newScore, ScoringConstants::MAX_SCORE);
  highScore = newScore;
  CreateScore(std::to_string(static_cast<int>(scoreAdd)), pos);
  SystemLocator::timerLocator->Start(2.0f, multTimer); // mult decreases after 2 seconds
  timerStarted = true;

  const float newMult = 0.1f * flowBonus * std::floor(mult) + mult;
  mult = std::min(newMult, ScoringConstants::MAX_MULT);
}

int ScoreSystem::GetEnemyScore(const EnemyType &id) {
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

void ScoreSystem::CreateScore(const std::string &score, const Vector2 &pos) {

  const size_t timer = SystemLocator::timerLocator->CreateTimer(0.7f, true);
  SystemLocator::renderLocator->AddScore({score, timer, pos});

}

void ScoreSystem::CreateMessage(const std::string &msg) {

  const size_t timer = SystemLocator::timerLocator->CreateTimer(0.0f, true);\
  SystemLocator::renderLocator->AddMessage({msg, timer});

}
