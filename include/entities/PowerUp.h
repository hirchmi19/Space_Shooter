#pragma once 
#include "PowerUpType.h"
#include "../../../raylib.h"


struct PowerUp : Entity {

    PowerUpType type;
    Movement1D movement;

};