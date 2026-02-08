//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once
#include <vector>

#include "raylib.h"

struct Sprite;


struct RenderData {

  std::vector<Sprite> sprites;
  Vector2 size;

};
