//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "entities/Player.h"
#include "locators/SystemLocator.h"
#include "entities/Player.h"

void Player::SetProjectileType(const ProjectileType &pType) {

    this->pType = pType;
    SystemLocator::timerLocator->Start(4.0f, projectileTimer);
}

void Player::SetPosition(const Vector2& pos) {

    movement.position = pos;
    combat.hitbox.x = pos.x;
    combat.hitbox.y = pos.y;
};


/**
 * Reads the player inputs and sets player speed
 */
void Player::Run() {

    int flowLvl = SystemLocator::scoreLocator->GetFlowLvl();
    
    movement.direction = 0;
    movement.speed = MovementConstants::BASE_SPEED;
    if (inFlowState) movement.speed = MovementConstants::FLOW_SPEED;
    if (SystemLocator::timerLocator->IsRunning(dashTimer)) movement.speed = MovementConstants::DASH_SPEED;
    if (!SystemLocator::timerLocator->IsRunning(projectileTimer)) pType = ProjectileType::BASE_PLAYER;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.direction = -1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.direction = 1;

    if (!SystemLocator::timerLocator->IsRunning(dashTimer) && 
    (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT))
    && inFlowState && flowLvl >= 2) {

        SystemLocator::timerLocator->Start(0.3f, dashTimer);
    }

    if (!SystemLocator::timerLocator->IsRunning(cooldownTimer) && IsKeyPressed(KEY_SPACE)) {

        const float shootCooldown = (inFlowState && flowLvl == 3 ) ? 0.1f : 0.35f;
        SystemLocator::entityLocator->SpawnProjectile(
            pType, movement.position, true);
        SystemLocator::timerLocator->Start(shootCooldown, cooldownTimer);
    }

}
