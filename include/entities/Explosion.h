#pragma once

#include <raylib.h>
#include <cstddef>

struct Explosion {

    Vector2 pos;
    size_t lifetime;
    Rectangle hitbox;
    RenderComponent render;
    bool active = true;

};
