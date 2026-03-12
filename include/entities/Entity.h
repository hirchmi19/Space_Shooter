#pragma once
#include <raylib.h>

#include "components/RenderComponent.h"

struct Entity {

    Vector2 position;
    RenderComponent render;
    Rectangle hitbox;
    bool isAlive = true;

};
