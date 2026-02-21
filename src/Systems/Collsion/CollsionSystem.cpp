//
// Created by Michael Hirsch on 21.02.26.
//

#include <complex>

#include "CollisionSystem.h"
#include "../../Game/GameWorld.h"

CollisionSystem::CollisionSystem() : IGameSystem(GameSystemID::COLLISION_SYSTEM){}

void CollisionSystem::Run(GameWorld &world) {

    CheckEnemiesProjectiles(world);

}


void CollisionSystem::CheckEnemiesProjectiles(GameWorld &world) {


    auto& enemies = world.GetEnemies();
    auto& projectiles = world.GetProjectiles();

    for (auto& enemy : enemies) {

        for (auto& projectile : projectiles) {

            if (CheckCollisionRecs(enemy.combat.hitbox, projectile.combat.hitbox) && projectile.movement.speed == -1) {

                enemy.combat.Kill();
                projectile.combat.Kill();
            }
        }
    }
}
