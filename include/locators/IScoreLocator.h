//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include "raylib.h"
#include "entities/PowerUpType.h"

enum class EnemyType;

struct IScoreLocator {

  virtual ~IScoreLocator() = default;

  virtual int GetEnemyScore(const EnemyType &id) = 0;
  virtual void AddHighScore(const int &score, const Vector2 &pos) = 0;
  virtual int GetFlowLvl() const = 0;
  virtual void ResetScore() = 0;
  virtual void ResetMult() = 0;
  virtual int GetHighScore() const = 0;
  virtual float GetMult() const = 0;
  virtual void RollPowerUpDrop() = 0;
};
