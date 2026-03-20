//
// Created by Michael Hirsch on 23.02.26.
//

#include "systems/scoring/ScoreSystem.h"

#include <cassert>
#include <iostream>
#include <ostream>

#include "constants/ScoringConstants.h"
#include "entities/enemies/EnemyType.h"
#include "locators/SystemLocator.h"
#include "systems/rendering/MessageUi.h"
#include "systems/scoring/LvlUpType.h"
#include "systems/timer/Timer.h"
#include "utils/utils.h"

ScoreSystem::ScoreSystem() : IGameSystem(GameSystemID::SCORE_SYSTEM, "SCORE_SYSTEM") {}


void ScoreSystem::Run() {

  if (mult >= baseMult + 2.0f && !flowStateActive) {
    
    SystemLocator::entityLocator->GetPlayer()->EnterFlowState();
    flowStateActive = true;
    CreateMessage("ENTERED FLOW STATE!");
  }

  if (!SystemLocator::timerLocator->IsRunning(multTimer) && timerStarted) {

    const float newMult = mult * 0.33f; // decrease mult by 2/3
    mult = std::max(newMult, baseMult); // mult cannot fall under base value

    SystemLocator::timerLocator->Start(TimerDurations::MULT_BUFF_DURATION, multTimer); // mult decreases after decrease as well

    if (!flowStateActive) return;

    SystemLocator::entityLocator->GetPlayer()->LeaveFlowState();
    CreateMessage("LEFT FLOW STATE!");
    ResetMult(); // reset mult back do default, after leaving the flow state
  }
}

/**
 * Calculates the real score and adds it to the highscore
 * \param score
 * \param pos
 */
void ScoreSystem::AddScore(const int &score, const Vector2 &pos) {

  flowBonus = flowStateActive ? 2 : 1; // double the points while in flow state

  const float scoreAdd = std::floor(static_cast<float>(score) * mult * flowBonus); // round down points
  int newScore = static_cast<int>(scoreAdd + static_cast<float>(highScore));
  newScore = std::min(newScore, ScoringConstants::MAX_SCORE); // cap score
  highScore = newScore;

  CreateScoreUi(std::to_string(static_cast<int>(scoreAdd)), pos);
  SystemLocator::timerLocator->Start(flowTime, multTimer);
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

    case EnemyType::PINK_E:
      return ScoringConstants::PINK_ENEMY_SCORE;

    case EnemyType::BLUE_E:
      return ScoringConstants::BLUE_ENEMY_SCORE;

    default:
      return 0;
  }
}

/**
 * Creates an score ui element for rendering
 * \param score
 * \param pos
 */
void ScoreSystem::CreateScoreUi(const std::string &score, const Vector2 &pos) {

  const size_t timer = SystemLocator::timerLocator->CreateTimer(TimerDurations::SCORE_DURATION, true);
  SystemLocator::renderLocator->AddScore({score, timer, pos});

}

/**
 * Creates an message ui element for rendering
 * \param msg
 */
void ScoreSystem::CreateMessage(const std::string &msg) {

  const size_t timer = SystemLocator::timerLocator->CreateTimer(0.0f, true);
  SystemLocator::renderLocator->AddMessage({msg, timer});

}

/**
 * Rolls wheter and if yes what type of power up is dropped
 * \return
 */
PowerUpType ScoreSystem::RollPowerUpDrop(){

   const int p = GetRandomValue(1, 6);
   if (p != 1) return PowerUpType::NONE; // 1/8 chance of dropping a power up

    const int numberOfPowerUps = AnyPowerUpsAvalaible() ? 3 : 2; // make sure no level up power ups spawn if there are no power ups left
    const int type = GetRandomValue(1, numberOfPowerUps); // 1/3 chance the power up being a level up, a shield or an projectile
    if (type == 1) return PowerUpType::RANDOM_PROJECTILE;
    if (type == 2) return PowerUpType::SHIELD;
    return PowerUpType::LEVEL_UP;

}

/**
 * Rolls what poewer up gets an level up
 * \return
 */
size_t ScoreSystem::RollLvlUp() {

  assert(AnyPowerUpsAvalaible() && "NO LEVEL UPS AVALAIBLE!"); // prevent possible endless loop

  size_t type;

  do {type = static_cast<size_t>(GetRandomValue(1, ToIndex(LvlUpType::COUNT) - 1));}
  while(!IsPowerUpAvalaible(ToEnum<LvlUpType>(type))); // loop is endless,
                                                           //if no power ups are left to be upgraded

  return type;
}

/**
 * Rolls an random projectile
 * \return
 */
const ProjectileType &ScoreSystem::RollProjectile() {

  const auto& start = ProjectileType::ARROW; // start position in the projectile type enum
  const size_t type = GetRandomValue(ToIndex(start), ToIndex(ProjectileType::COUNT) - 1);
  return ToEnum<ProjectileType>(type);

}

/**
 * Applies an power up to the player
 * \param powType
 */
void ScoreSystem::ApplyPowerUp(const PowerUpType &powType) {

  Player* player = SystemLocator::entityLocator->GetPlayer();
  auto& shield = SystemLocator::entityLocator->GetShield();

  if (powType == PowerUpType::LEVEL_UP) ExecuteLvlUp(RollLvlUp());
  else if (powType == PowerUpType::SHIELD) {

    player->ActivateShield();
    shield.hp = shield.lvl;
  }

  else {

    const ProjectileType& prType = RollProjectile();
    SystemLocator::entityLocator->GetPlayer()->SetProjectileType(prType);
  }

}

/**
 * Levels up an power up
 * \param index
 */
void ScoreSystem::ExecuteLvlUp(const size_t &index) {

  if (index == 0 || !AnyPowerUpsAvalaible()) return;

  const auto type = ToEnum<LvlUpType>(index);

  if (type == LvlUpType::FLOW) LvlFlowState();
  else if (type == LvlUpType::SHIELD) LvlShield();
  else if (type == LvlUpType::MULT) LvlDefaultMult();
  else {

    SystemLocator::entityLocator->LvlProjectiles();
    CreateMessage("PROJECTILES UP!");

  }
}

void ScoreSystem::LvlFlowState() {

  if (flowLvl >= ScoringConstants::MAX_FLOW_LEVEL) return;

  flowLvl++;
  maxMult *= 2.5f;
  flowTime += 2.0f;
  CreateMessage("FLOW STATE UP!");


}

void ScoreSystem::LvlShield() {

  auto& shield = SystemLocator::entityLocator->GetShield();
  if (shield.lvl >= ScoringConstants::MAX_SHIELD_LEVEL) return;

  shield.lvl++;
  shield.hp++;
  CreateMessage("SHIELD UP!");
}

void ScoreSystem::LvlDefaultMult() {

  if (baseMult >= ScoringConstants::MAX_BASE_MULT) return;
  baseMult++;
  mult = baseMult;
  CreateMessage("MULT UP!");
}

/**
 * Returns true if ther is any type of power up, which isnt max level yet
 * \return
 */
bool ScoreSystem::AnyPowerUpsAvalaible() {

  if (IsPowerUpAvalaible(LvlUpType::MULT)) return true;
  if (IsPowerUpAvalaible(LvlUpType::SHIELD)) return true;
  if (IsPowerUpAvalaible(LvlUpType::FLOW)) return true;
  if (IsPowerUpAvalaible(LvlUpType::PROJECTILE)) return true;

  return false;
}

/**
 * Returns true if an given power up isnt max level yet
 * \param type
 * \return
 */
bool ScoreSystem::IsPowerUpAvalaible(const LvlUpType &type) {

  const auto& shield = SystemLocator::entityLocator->GetShield();

  switch (type) {

    case LvlUpType::SHIELD:
      return shield.lvl < ScoringConstants::MAX_SHIELD_LEVEL;

    case LvlUpType::MULT:
      return baseMult < ScoringConstants::MAX_BASE_MULT;

    case LvlUpType::FLOW:
      return flowLvl < ScoringConstants::MAX_FLOW_LEVEL;

    case LvlUpType::PROJECTILE:
      return SystemLocator::entityLocator->GetProjectileHp() < ScoringConstants::MAX_PROJECTILE_LEVEL;

    case LvlUpType::COUNT: case LvlUpType::NONE: return false; // fall through these cases, as they are irrelevant
  }

  return false;
}




