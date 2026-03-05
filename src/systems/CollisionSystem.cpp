//
// Created by Michael Hirsch on 21.02.26.
//


#include "components/WaveState.h"
#include "systems/collision/CollisionSystem.h"
#include "locators/SystemLocator.h"

CollisionSystem::CollisionSystem() : IGameSystem(GameSystemID::COLLISION_SYSTEM, "COLLISION_SYSTEM"){}

void CollisionSystem::Run() {

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

            if (CheckCollisionRecs(enemy.combat.hitbox, projectile.combat.hitbox) && projectile.isPlayerProjectile) {

                enemy.combat.TakeDamage(projectile.combat.damage);
                SystemLocator::entityLocator->SpawnPowerUp(PowerUpType::LEVEL_UP, enemy.wave.worldPosition);
                projectile.combat.TakeDamage(projectile.combat.damage);
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