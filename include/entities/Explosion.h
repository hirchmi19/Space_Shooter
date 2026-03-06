#pragma once

#include <raylib.h>
#include <cstddef>

struct Explosion {

    Vector2 center;
    size_t timer;
    float radius;
    float damage;
    RenderComponent render;
    bool active = true;


};
