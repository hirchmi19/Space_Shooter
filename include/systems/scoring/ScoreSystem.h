//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include "raylib.h"
#include "../../entities/enemies/EnemyType.h"
#include "../../game/IGameSystem.h"
#include "../../locators/IScoreLocator.h"
#include <string>

#include "LvlUpType.h"
#include "entities/PowerUpType.h"
#include "entities/projectiles/ProjectileType.h"


class ScoreSystem : public IScoreLocator, public IGameSystem {

public:
  ScoreSystem();

  ~ScoreSystem() override = default;

  void Run() override;

  int GetHighScore() const override { return highScore; }
  float GetMult() const override { return mult; }
  int GetFlowLvl() const override { return flowLvl; }

  void AddHighScore(const int &score, const Vector2 &pos) override;
  int GetEnemyScore(const EnemyType &id) override;

  PowerUpType RollPowerUpDrop() override;

  size_t RollLvlUp();
  void ExecuteLvlUp(const size_t &Index);
  void ApplyPowerUp(const PowerUpType& powType) override;
  void LevelUp();

  void ResetScore() override {
    highScore = 0;
    mult = multDefault = 1.0f;
    flowLvl = 1;
    maxMult = 10;
    flowStateActive = false;
  }

  void ResetMult() override {
    mult = multDefault;
    timerStarted = false;
    flowStateActive = false;
  }


private:

  int flowLvl = 1;
  int highScore = 0;
  float multDefault = 1.0f;
  float mult = multDefault;
  float maxMult = 10;
  float flowBonus{};
  float flowTime = 2.0f;

  size_t multTimer{};
  size_t flowTimer{};

  bool timerStarted = false;
  bool flowStateActive = false;

  void CreateScoreUi(const std::string &score, const Vector2 &pos);
  void CreateMessage(const std::string &msg);

  void LvlFlowState();
  void LvlShield();
  void LvlDefaultMult();

  bool AnyPowerUpsAvalaible();
  bool IsPowerUpAvalaible(const LvlUpType& type);

  const ProjectileType& RollProjectile();

};
