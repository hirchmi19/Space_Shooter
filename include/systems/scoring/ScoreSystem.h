//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include <vector>
#include "raylib.h"
#include "../../entities/enemies/EnemyType.h"
#include "../../game/IGameSystem.h"
#include "../../locators/IScoreLocator.h"


class ScoreSystem : public IScoreLocator, public IGameSystem {

public:
  ScoreSystem();

  ~ScoreSystem() override = default;

  void Run() override;

  int GetHighScore() const override { return highScore; }
  float GetMult() const override { return mult; }

  void AddHighScore(const int &score, const Vector2 &pos) override;

  int GetEnemyScore(const EnemyType &id) override;

  void ResetScore() override {
    highScore = 0;
    mult = multDefault;
    timerStarted = false;
  }

  void ResetMult() override {
    mult = multDefault;
    timerStarted = false;
  }


private:
  int highScore = 0;
  float multDefault = 1.0f;
  float mult = multDefault;
  float flowBonus{};

  size_t multTimer{};
  size_t flowTimer{};

  bool timerStarted = false;
  bool flowStateActive = false;

  void CreateScoreUi(const std::string &score, const Vector2 &pos);
  void CreateMessage(const std::string &msg);
};
