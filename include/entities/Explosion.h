#pragma once

#include <raylib.h>
#include <cstddef>

struct Explosion : Entity {

    size_t lifetime;
    bool active = true;

};
