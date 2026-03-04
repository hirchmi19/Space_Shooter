#pragma once
#include "raylib.h"
#include <cstddef>
#include "constants/GameWorldConstants.h"

struct MessageUi {

  std::string text;
  size_t displayTimer{};
  Vector2 pos;
  bool active = false;
  int fSize = 18;
};
