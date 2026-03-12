//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include "raylib.h"

enum class TextureID;
enum class SpriteID;

struct Sprite {

    SpriteID sprite;
    TextureID texture;
    Rectangle src;
};
