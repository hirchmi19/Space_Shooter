//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "entities/Player.h"
#include "locators/SystemLocator.h"

void Player::SetPosition(const Vector2& pos) {

    movement.position = pos;
    combat.hitbox.x = pos.x;
    combat.hitbox.y = pos.y;
};


/**
 * Reads the player inputs
 */
void Player::HandleInput() {

    movement.speed = 0;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.speed = -1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.speed = 1;

    if (SystemLocator::timerLocator->IsRunning(cooldownTimer)) return;

    if (IsKeyPressed(KEY_SPACE)) {

        SystemLocator::entityLocator->SpawnProjectile(
            ProjectileType::PLAYER,
            movement.position, true);
        SystemLocator::timerLocator->Start(0.35f, cooldownTimer);
    }

}
