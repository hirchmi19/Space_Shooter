//
// Created by Michael Hirsch on 09.02.26.
//

#include "systems/movement/MovementSystem.h"
#include "locators/SystemLocator.h"
#include "constants/RenderConstants.h"
#include "constants/GameConstants.h"

MovementSystem::MovementSystem() : IGameSystem(GameSystemID::MOVEMENT_SYSTEM, "MOVEMENT_SYSTEM") {

}

void MovementSystem::Run() {

    MovePlayer();
    MoveEnemies();
    MoveProjectiles();
    MovePowerUps();
}

//--------------------------------------------------------------------------


/**
 * Moves the player
 */
void MovementSystem::MovePlayer() {

    Player* player  = SystemLocator::entityLocator->GetPlayer();
    auto& shield    = SystemLocator::entityLocator->GetShield();
    const int dir   = player->GetDir();
    Vector2 pos     = player->GetPosition();
    const float rightEdge = pos.x + player->GetSize().x * RenderConstants::PLAYER_SCALING;

    if (rightEdge > GameConstants::SCREEN_WIDTH && dir > 0) return;
    if (pos.x <= 0 && dir < 0) return;

    pos.x += static_cast<float>(dir) * player->GetSpeed();
    player->SetPosition(pos);

    // sync shield to player center
    shield.position.x = pos.x + (player->GetSize().x * RenderConstants::PLAYER_SCALING) / 2 -
                        (shield.render.size.x * RenderConstants::SHIELD_SCALING) / 2;
    shield.hitbox.x = shield.position.x;

}

/**
 * Moves all projectiles
 */
void MovementSystem::MoveProjectiles() {

    auto& projectiles = SystemLocator::entityLocator->GetProjectiles();

    for (auto& projectile : projectiles) {

        const float bottomEdge = projectile.position.y + projectile.render.size.y;
        if (projectile.position.y <= 0 || bottomEdge >= GameConstants::SCREEN_HEIGHT) {
            projectile.isAlive = false;
            continue;
        }

        MoveProjectile(projectile);
    }
}

void MovementSystem::MovePowerUps() {

    auto& powerUps = SystemLocator::entityLocator->GetPowerUps();

    for (auto& powerUp : powerUps) {

        if (powerUp.position.y >= GameConstants::SCREEN_HEIGHT) {
            powerUp.isAlive = false;
            continue;
        }

        powerUp.position.y += static_cast<float>(powerUp.movement.direction) * powerUp.movement.speed;
        powerUp.hitbox.x = powerUp.position.x;
        powerUp.hitbox.y = powerUp.position.y;
    }
}

/**
 * Moves all enemies
 */
void MovementSystem::MoveEnemies() {

    auto& enemies = SystemLocator::entityLocator->GetEnemies();

    for (auto& enemy : enemies) {

        if (enemy.wave.state != WaveState::ENTER_FORMATION &&
            enemy.wave.state != WaveState::ATTACK) continue;

        if (enemy.wave.t >= 1.0f && enemy.wave.state == WaveState::ENTER_FORMATION) {
            MoveEnemy(enemy, enemy.wave.formationPosition);
            enemy.wave.state = WaveState::IN_FORMATION;
            continue;
        }

        if (enemy.wave.t >= 1.0f && enemy.wave.state == WaveState::ATTACK) {
            MoveEnemy(enemy, enemy.wave.worldPosition);
            enemy.wave.state = WaveState::OUT_FORMATION;
            continue;
        }

        enemy.wave.t += enemy.wave.speed * (1.0f / GameConstants::UPS);

        const auto& cp = enemy.wave.controlPoints;
        MoveEnemy(enemy, GetSplinePointBezierCubic(cp[0], cp[1], cp[2], cp[3], enemy.wave.t));
    }
}


void MovementSystem::MoveProjectile(Projectile& projectile) {

    projectile.position.y += static_cast<float>(projectile.movement.direction) * projectile.movement.speed;
    projectile.hitbox.x = projectile.position.x;
    projectile.hitbox.y = projectile.position.y;

}

void MovementSystem::MoveEnemy(Enemy &enemy, const Vector2& newPos) {

    enemy.position = newPos;
    enemy.wave.worldPosition = newPos;
    enemy.hitbox.x = newPos.x;
    enemy.hitbox.y = newPos.y;

}
