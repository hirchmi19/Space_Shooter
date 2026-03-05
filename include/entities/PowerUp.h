#pragma once 
#include "../../../raylib.h"


struct PowerUp {

    Movement1D movement;
    RenderComponent render;
    Rectangle hitbox;
    bool alive;

};