//
// Created by Michael Hirsch on 21.02.26.
//

#include "CollisionSystem.h"
#include "../../Game/GameWorld.h"

CollisionSystem::CollisionSystem() : IGameSystem(GameSystemID::COLLISION_SYSTEM, "COLLISION_SYSTEM"){}

void CollisionSystem::Run(GameWorld &world) {

    CheckEnemiesProjectiles(world);
    CheckPlayerProjectiles(world);
    CheckPlayerEnemies(world);
}

//--------------------------------------------------------------------------

void CollisionSystem::CheckEnemiesProjectiles(GameWorld &world) {


    auto& enemies = world.GetEnemies();
    auto& projectiles = world.GetProjectiles();

    for (auto& enemy : enemies) {

        for (auto& projectile : projectiles) {

            if (CheckCollisionRecs(enemy.combat.hitbox, projectile.combat.hitbox) && projectile.isPlayerProjectile) {

                enemy.combat.Kill();
                projectile.combat.Kill();
            }
        }
    }
}

void CollisionSystem::CheckPlayerEnemies(GameWorld &world) {

    const auto& enemies = world.GetEnemies();
    auto& player = world.GetPlayer();

    for (auto& enemy : enemies) {

        if (enemy.wave.state != WaveState::ATTACK) continue;
        if (CheckCollisionRecs(enemy.combat.hitbox, player.GetHitBox())) player.Kill();
    }
}

void CollisionSystem::CheckPlayerProjectiles(GameWorld &world) {

    const auto& projectiles = world.GetProjectiles();
    auto& player = world.GetPlayer();

    for (auto& projectile : projectiles) {

        if (projectile.movement.position.y < player.GetPosition().y) continue;
        if (CheckCollisionRecs(projectile.combat.hitbox, player.GetHitBox()) && !projectile.isPlayerProjectile) player.Kill();
    }

}