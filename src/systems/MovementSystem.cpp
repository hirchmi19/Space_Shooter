//
// Created by Michael Hirsch on 09.02.26.
//

#include "systems/movement/MovementSystem.h"
#include "locators/SystemLocator.h"
#include "constants/MovementConstants.h"
#include "constants/RenderConstants.h"
#include "constants/GameConstants.h"

MovementSystem::MovementSystem() : IGameSystem(GameSystemID::MOVEMENT_SYSTEM, "MOVEMENT_SYSTEM"){}

void MovementSystem::Run() {

    MovePlayer();
    MoveEnemies();
    MoveProjectiles();
}

//--------------------------------------------------------------------------


/**
 * Moves the player
 */
void MovementSystem::MovePlayer() {

    Player* player = SystemLocator::entityLocator->GetPlayer();
    const auto& playerSpeed = player->GetSpeed();
    Vector2 playerPosition = player->GetPosition();
    const float playerPosXRight = playerPosition.x + player->GetSize().x * RenderConstants::PLAYER_SCALING;

    if (playerPosXRight > GameConstants::SCREEN_WIDTH && playerSpeed > 0)return;
    if (playerPosition.x <= 0 && playerSpeed < 0) return;

    const float playerWorldSpeed = player->IsInFlowState() ? 8 : 6;
    playerPosition.x += static_cast<float>(player->GetSpeed()) * playerWorldSpeed;
    player->SetPosition(playerPosition);
}

/**
 * Moves all projectiles
 * \param world
 */
void MovementSystem::MoveProjectiles() {

   auto& projectiles = SystemLocator::entityLocator->GetProjectiles();

    for (auto& projectile : projectiles) {

        const float posYUp = projectile.movement.position.y + projectile.render.size.y;

        if (projectile.movement.position.y <= 0 || posYUp >= GameConstants::SCREEN_HEIGHT)
            projectile.combat.Kill();

        MoveProjectile(projectile);
    }
}

/**
 * Moves all enemies
 * \param world
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

        enemy.wave.t += enemy.wave.speed * GetFrameTime();

        auto& bezierPoints = enemy.wave.controlPoints;
        const Vector2 newPos = GetSplinePointBezierCubic(bezierPoints[0],
            bezierPoints[1],
            bezierPoints[2],
            bezierPoints[3],
            enemy.wave.t);

        MoveEnemy(enemy, newPos);
    }
}


void MovementSystem::MoveProjectile(Projectile& projectile) {

    projectile.movement.position.y += static_cast<float>(projectile.movement.speed) * MovementConstants::PROJECTILE_WORLD_SPEED;
    projectile.combat.hitbox.x = projectile.movement.position.x;
    projectile.combat.hitbox.y = projectile.movement.position.y;

}

void MovementSystem::MoveEnemy(Enemy &enemy, const Vector2& newPos) {

    enemy.wave.worldPosition = newPos;
    enemy.combat.hitbox.x = newPos.x;
    enemy.combat.hitbox.y = newPos.y;

}
