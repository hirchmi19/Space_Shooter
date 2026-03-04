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
 * Reads the player inputs and sets player speed
 */
void Player::Run() {

    movement.direction = 0;
    movement.speed = MovementConstants::BASE_SPEED;
    if (inFlowState) movement.speed = MovementConstants::FLOW_SPEED;
    if (SystemLocator::timerLocator->IsRunning(dashTimer)) movement.speed = MovementConstants::DASH_SPEED;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.direction = -1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.direction = 1;

    if (!SystemLocator::timerLocator->IsRunning(dashTimer) && IsKeyPressed(KEY_LEFT_SHIFT) && inFlowState && flowLvl >= 2) {

        SystemLocator::timerLocator->Start(0.3f, dashTimer);
    }

    if (!SystemLocator::timerLocator->IsRunning(cooldownTimer) && IsKeyPressed(KEY_SPACE)) {

        SystemLocator::entityLocator->SpawnProjectile(
            ProjectileType::PLAYER,
            movement.position, true);
        SystemLocator::timerLocator->Start(0.35f, cooldownTimer);
    }

}
