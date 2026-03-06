//
// Created by Michael Hirsch on 23.02.26.
//

#include "systems/scoring/ScoreSystem.h"
#include "constants/ScoringConstants.h"
#include "entities/enemies/EnemyType.h"
#include "locators/SystemLocator.h"
#include "systems/rendering/MessageUi.h"
#include "systems/scoring/LvlUpType.h"
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
  mult = std::min(newMult, maxMult);
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

PowerUpType ScoreSystem::RollPowerUpDrop(){

    int p = GetRandomValue(1, 8);
    if (p != 1) return PowerUpType::NONE; // 1/8 chance of dropping a power up

    int type = GetRandomValue(0, 1); // 1/3 chance the power up being a level up, a shield or an projectile
    if (type == 0 && AnyPowerUpsAvalaible()) return PowerUpType::LEVEL_UP;
    if (type == 1) return PowerUpType::SHIELD;
    return ToEnum<PowerUpType>(GetRandomValue(4, ToIndex(PowerUpType::COUNT) - 1)); // pick an random projectile

}

size_t ScoreSystem::RollLvlUp() {

  if (!AnyPowerUpsAvalaible()) return 0; // important!

  size_t type;

  do {type = static_cast<size_t>(GetRandomValue(1, ToIndex(LvlUpType::COUNT) - 1));}
  while(!IsPowerUpAvalaible(ToEnum<LvlUpType>(type))); // loop is endless,
                                                           //if no power ups are left to be upgraded

  return type;
}

void ScoreSystem::ApplyPowerUp(const PowerUpType &powType) {

  Player* player = SystemLocator::entityLocator->GetPlayer();
  auto& shield = SystemLocator::entityLocator->GetShield();

  if (powType == PowerUpType::LEVEL_UP) ExecuteLvlUp(RollLvlUp());
  else if (powType == PowerUpType::SHIELD) {

    player->ActivateShield();
    shield.hp = shield.lvl;
  }

  // add projectiles
}

void ScoreSystem::ExecuteLvlUp(const size_t &index) {

  if (index == 0 || !AnyPowerUpsAvalaible()) return;

  auto type = ToEnum<LvlUpType>(index);

  if (type == LvlUpType::FLOW) LvlFlowState();
  else if (type == LvlUpType::SHIELD) LvlShield();
  else if (type == LvlUpType::MULT) LvlDefaultMult();
  else {

    SystemLocator::entityLocator->LvlProjectiles();
    CreateMessage("PROJECTILES UP!");

  }
}

void ScoreSystem::LvlFlowState() {

  if (flowLvl <= 3) {

    flowLvl++;
    maxMult *= 2.5f;
    CreateMessage("FLOW STATE UP!");

  }
}

void ScoreSystem::LvlShield() {

  auto& shield = SystemLocator::entityLocator->GetShield();
  if (shield.lvl >= 3) return;

  shield.lvl++;
  shield.hp++;
  CreateMessage("SHIELD UP!");
}

void ScoreSystem::LvlDefaultMult() {

  if (multDefault >= ScoringConstants::MAX_DEFAULT_MULT) return;
  multDefault++;
  CreateMessage("MULT UP!");
}

bool ScoreSystem::AnyPowerUpsAvalaible() {

  const auto& shield = SystemLocator::entityLocator->GetShield();

  if (multDefault >= ScoringConstants::MAX_DEFAULT_MULT) return false;
  if (shield.lvl >= 3) return false;
  if (flowLvl >= 3) return false;
  if (SystemLocator::entityLocator->GetProjectileHp() >= 3) return false;

  return true;
}

bool ScoreSystem::IsPowerUpAvalaible(const LvlUpType &type) {

  if (type == LvlUpType::COUNT || type == LvlUpType::NONE) return false;

  const auto& shield = SystemLocator::entityLocator->GetShield();

  switch (type) {

    case LvlUpType::SHIELD:

      if (shield.lvl >= 3) return false;
      return true;

    case LvlUpType::MULT:
      if (multDefault >= ScoringConstants::MAX_DEFAULT_MULT) return false;
      return true;

    case LvlUpType::FLOW:

      if (flowLvl >= 3) return false;
      return true;

    case LvlUpType::PROJECTILE:

      if (SystemLocator::entityLocator->GetProjectileHp() >= 3) return false;
      return true;
  }

  return false;
}



