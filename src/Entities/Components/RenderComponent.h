//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once
#include <vector>

#include "raylib.h"

struct Sprite;


struct RenderComponent {

  std::vector<const Sprite*> sprites;
  Vector2 size;

};
