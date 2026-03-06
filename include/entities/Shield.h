#pragma once
#include "raylib.h"
#include "../components/RenderComponent.h"
#include "components/CombatComponent.h"

struct Shield {

  Vector2 position;
  RenderComponent render;
  size_t cooldown;
  Rectangle hitbox{};
  int hp = 1;
  int lvl = 1;

};
