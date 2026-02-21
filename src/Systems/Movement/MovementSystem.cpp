//
// Created by Michael Hirsch on 09.02.26.
//

#include "MovementSystem.h"

#include <iostream>
#include <ostream>

#include "../../Constants/MovementConstants.h"
#include "../../Game/GameWorld.h"
#include "../../Constants/RenderConstants.h"

MovementSystem::MovementSystem() : IGameSystem(GameSystemID::MOVEMENT_SYSTEM){}

void MovementSystem::Run(GameWorld& world) {

    MovePlayer(world);
    MoveEnemies(world);
    MoveProjectiles(world);
}

//--------------------------------------------------------------------------


/**
 * Moves the player
 * \param world
 */
void MovementSystem::MovePlayer(GameWorld& world) {

    Player& player = world.GetPlayer();
    const auto& playerSpeed = player.GetSpeed();
    const Vector2& playerPosition = player.GetPosition();
    const uint32_t playerPosXRight = playerPosition.x + player.GetSize().x * RenderConstants::PLAYER_SCALING;

    if (playerPosXRight > GameConstants::SCREEN_WIDTH && playerSpeed > 0)return;
    if (playerPosition.x <= 0 && playerSpeed < 0) return;

    MovePlayer(player);
}

/**
 * Moves all projectiles
 * \param world
 */
void MovementSystem::MoveProjectiles(GameWorld& world) {

   auto& projectiles = world.GetProjectiles();

    for (auto& projectile : projectiles) {

        const int posYUp = projectile.movement.position.y + projectile.render.size.y;

        if (projectile.movement.position.y <= 0 || posYUp >= GameConstants::SCREEN_HEIGHT)
            projectile.combat.Kill();

        MoveProjectile(projectile);
    }
}

/**
 * Moves all enemies
 * \param world
 */
void MovementSystem::MoveEnemies(GameWorld &world) {

    auto& enemies = world.GetEnemies();

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
        const Vector2 newPos = GetSplinePointBezierCubic(bezierPoints[0], bezierPoints[1], bezierPoints[2], bezierPoints[3], enemy.wave.t);
        MoveEnemy(enemy, newPos);
    }




}

void MovementSystem::MovePlayer(Player& player) {

    Vector2 playerPosition = player.GetPosition();
    playerPosition.x += player.GetSpeed() * MovementConstants::PLAYER_WORLD_SPEED;
    player.SetPosition(playerPosition);
}

void MovementSystem::MoveProjectile(Projectile& projectile) {

    projectile.movement.position.y += projectile.movement.speed * MovementConstants::PROJECTILE_WORLD_SPEED;
    projectile.combat.hitbox.x = projectile.movement.position.x;
    projectile.combat.hitbox.y = projectile.movement.position.y;

}

void MovementSystem::MoveEnemy(Enemy &enemy, const Vector2& newPos) {

    enemy.wave.worldPosition = newPos;
    enemy.combat.hitbox.x = newPos.x;
    enemy.combat.hitbox.y = newPos.y;

}
