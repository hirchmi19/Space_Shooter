#pragma once
#include <cstddef>
#include "raylib.h"
#include "entities/Entity.h"
#include "../components/RenderComponent.h"
#include "components/CombatComponent.h"

struct Shield : Entity {

  size_t cooldown;
  int hp = 3;
  int lvl = 3;

};
