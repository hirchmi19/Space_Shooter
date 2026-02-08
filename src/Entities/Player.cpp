//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Player.h"

#include "../GameWorld.h"


void Player::HandleInput(GameWorld& world) {

    movement.speed = 0;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.speed--;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.speed++;

    if (IsKeyPressed(KEY_BACKSPACE)) world.SpawnProjectile(SpriteID::PLAYER_PROJECTILE, movement.position);
}
