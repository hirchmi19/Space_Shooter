//
// Created by Michael Hirsch on 21.02.26.
//

#include "CollisionSystem.h"
#include "../../Game/GameWorld.h"
#include "Entities/Components/WaveState.h"
#include "Entities/Projectile.h"
#include "Entities/Enemy.h"

CollisionSystem::CollisionSystem() : IGameSystem(GameSystemID::COLLISION_SYSTEM, "COLLISION_SYSTEM"){}

void CollisionSystem::Run(GameWorld &world) {

    CheckEnemiesProjectiles();
    CheckPlayerProjectiles();
    CheckPlayerEnemies();
}

//--------------------------------------------------------------------------

void CollisionSystem::CheckEnemiesProjectiles() {

    auto& enemies = SystemLocator::entityLocator->GetEnemies();
    auto& projectiles = SystemLocator::entityLocator->GetProjectiles();

    for (auto& enemy : enemies) {

        for (auto& projectile : projectiles) {

            if (CheckCollisionRecs(enemy.combat.hitbox, projectile.combat.hitbox)) {

                enemy.combat.Kill();
                projectile.combat.Kill();
            }
        }
    }
}

void CollisionSystem::CheckPlayerEnemies() {

    const auto& enemies= SystemLocator::entityLocator->GetEnemies();
    Player *player = SystemLocator::entityLocator->GetPlayer();

    for (const auto& enemy : enemies) {

        if (enemy.wave.state != WaveState::ATTACK) continue;
        if (CheckCollisionRecs(enemy.combat.hitbox, player->GetHitBox())) player->Kill();
    }
}

void CollisionSystem::CheckPlayerProjectiles() {

    const auto& projectiles = SystemLocator::entityLocator->GetProjectiles();
    Player *player = SystemLocator::entityLocator->GetPlayer();

    for (const auto& projectile : projectiles) {

        if (projectile.movement.position.y < player->GetPosition().y) continue;
        if (CheckCollisionRecs(projectile.combat.hitbox, player->GetHitBox()) && !projectile.isPlayerProjectile) player->Kill();
    }

}