#pragma once

#include <raylib.h>
#include <cstddef>
#include "Entity.h"

struct Explosion : Entity {

    size_t lifetime;
    bool active = true;

};
