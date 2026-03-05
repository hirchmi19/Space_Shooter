#pragma once 
#include "../../../raylib.h"


struct PowerUp {

    PowerUpType type;
    Movement1D movement;
    RenderComponent render;
    Rectangle hitbox;
    bool alive;

};