//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Player.h"

#include <iostream>
#include <ostream>

#include "../Game/GameWorld.h"


void Player::HandleInput(GameWorld& world) {

    movement.speed = 0;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {

        if (movement.speed <= -1) return;
        movement.speed--;
        std::cout << movement.speed << std::endl;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {

        if (movement.speed >= 1) return;
        movement.speed++;
        std::cout << movement.speed << std::endl;
    }

    if (IsKeyPressed(KEY_SPACE)) {

        std::cout << "shoot fired" << std::endl;
        world.SpawnPlayerProjectile(movement.position);
    }
}
