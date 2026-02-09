//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Player.h"

#include <iostream>
#include <ostream>

#include "../GameWorld.h"


void Player::CalcNewPosition(const int32_t& speed) {

    movement.position.x += speed;
    combat.hitbox.x = movement.position.x;
    combat.hitbox.y = movement.position.y;
}

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

    if (IsKeyPressed(KEY_SPACE)) world.SpawnProjectile(SpriteID::PLAYER_PROJECTILE, movement.position);
}
