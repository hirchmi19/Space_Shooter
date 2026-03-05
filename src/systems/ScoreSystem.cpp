//
// Created by Michael Hirsch on 23.02.26.
//

#include "systems/scoring/ScoreSystem.h"
#include "constants/ScoringConstants.h"
#include "entities/enemies/EnemyType.h"
#include "locators/SystemLocator.h"
#include "systems/rendering/MessageUi.h"
#include "utils/utils.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM") {}


void ScoreSystem::Run() {

  if (mult >= 2.0f && !flowStateActive) {
    
    SystemLocator::entityLocator->GetPlayer()->EnterFlowState();
    flowStateActive = true;
    CreateMessage("ENTERED FLOW STATE!");
  }

  if (!SystemLocator::timerLocator->IsRunning(multTimer) && timerStarted) {

    const float newMult = mult * 0.33f; // decrease mult
    mult = std::max(newMult, multDefault); // mult cannot fall under default value

    SystemLocator::timerLocator->Start(3.0f, multTimer); // mult decreases after decrease as well

    if (!flowStateActive) return;

    SystemLocator::entityLocator->GetPlayer()->LeaveFlowState();
    CreateMessage("LEAVED FLOW STATE!");
    ResetMult(); // reset mult back do default, after leaving the flow state
    flowStateActive = false;
  }
}

void ScoreSystem::AddHighScore(const int &score, const Vector2 &pos) {

  flowBonus = flowStateActive ? 2 : 1; // double the points while in flow state

  const float scoreAdd = std::floor(static_cast<float>(score) * mult * flowBonus); // round down points
  int newScore = static_cast<int>(scoreAdd + static_cast<float>(highScore));
  newScore = std::min(newScore, ScoringConstants::MAX_SCORE); // cap score
  highScore = newScore;

  CreateScoreUi(std::to_string(static_cast<int>(scoreAdd)), pos);
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

void ScoreSystem::CreateScoreUi(const std::string &score, const Vector2 &pos) {

  const size_t timer = SystemLocator::timerLocator->CreateTimer(0.7f, true);
  SystemLocator::renderLocator->AddScore({score, timer, pos});

}

void ScoreSystem::CreateMessage(const std::string &msg) {

  const size_t timer = SystemLocator::timerLocator->CreateTimer(0.0f, true);\
  SystemLocator::renderLocator->AddMessage({msg, timer});

}

void ScoreSystem::RollPowerUpDrop(){




}

void ScoreSystem::ApplyPowerUp(const PowerUpType &powType) {

  Player* player = SystemLocator::entityLocator->GetPlayer();

  if (powType == PowerUpType::LEVEL_UP) ExecuteLvlUp(RollLvlUp());
  else if (powType == PowerUpType::SHIELD && !player->IsShieldActive()) player->ActivateShield();

  //TODO: add projectiles

}

size_t ScoreSystem::RollLvlUp() {

  return static_cast<size_t>(GetRandomValue(0, ToIndex(LvlUpType::COUNT) - 1));
}

void ScoreSystem::ExecuteLvlUp(const size_t &lvlIndex) {

  auto lvl = ToEnum<LvlUpType>(lvlIndex);

  switch (lvl) {

    case LvlUpType::FLOW_STATE:

      if (flowLvl <= 3) flowLvl++;
      else flowLvl = 3;
      break;

    case LvlUpType::SHIELD:

      auto& shield = SystemLocator::entityLocator->GetShield();
      if (shield.lvl >= 3) return;

        shield.lvl++;
        shield.hp++;
      break;

    //TODO: add projectiles
  }
}
