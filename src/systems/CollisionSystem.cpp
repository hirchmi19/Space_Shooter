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
    CheckPowerUps();
}

//--------------------------------------------------------------------------

void CollisionSystem::CheckEnemiesProjectiles() {

    auto& enemies = SystemLocator::entityLocator->GetEnemies();
    auto& projectiles = SystemLocator::entityLocator->GetProjectiles();

    for (auto& enemy : enemies) {

        for (auto& projectile : projectiles) {

            if (CheckCollisionRecs(enemy.combat.hitbox, projectile.combat.hitbox) && projectile.isPlayerProjectile) {

                const auto& powerUp = SystemLocator::scoreLocator->RollPowerUpDrop();
                SystemLocator::entityLocator->SpawnPowerUp(powerUp, enemy.wave.worldPosition);\

                enemy.combat.TakeDamage(projectile.combat.damage); // enmy always dies

                if (projectile.type == ProjectileType::ARROW) { // arrow pierces through multiple enmies

                    projectile.combat.TakeDamage(0.5f);
                    continue;
                }

                if (projectile.type == ProjectileType::ROCKET)
                    SystemLocator::entityLocator->SpawnExplosion(enemy.wave.worldPosition);

                if (projectile.type != ProjectileType::GLAIVE) projectile.combat.Kill(); // glaive meltes through everything
            }
        }
    }
}

void CollisionSystem::CheckPlayerEnemies() {

    auto& enemies= SystemLocator::entityLocator->GetEnemies();
    Player *player = SystemLocator::entityLocator->GetPlayer();
    auto& shield =  SystemLocator::entityLocator->GetShield();

    for (auto& enemy : enemies) {

        if (enemy.wave.state != WaveState::ATTACK) continue;
        if (CheckCollisionRecs(enemy.combat.hitbox, player->GetHitBox())) {

            if (!player->IsShieldActive()) {

                player->Kill();
                return;
            }

            if (!SystemLocator::timerLocator->IsRunning(shield.cooldown)) {

                shield.hp--;
                SystemLocator::timerLocator->Start(0.5f, shield.cooldown);
            }

            enemy.combat.Kill();
        }
    }
}

void CollisionSystem::CheckPlayerProjectiles() {

    auto& projectiles = SystemLocator::entityLocator->GetProjectiles();
    Player *player = SystemLocator::entityLocator->GetPlayer();
    auto& shield =  SystemLocator::entityLocator->GetShield();

    for (auto& projectile : projectiles) {

        if (projectile.movement.position.y < player->GetPosition().y) continue;
        if (CheckCollisionRecs(projectile.combat.hitbox, player->GetHitBox()) && !projectile.isPlayerProjectile) {

            if (!player->IsShieldActive()) {

                player->Kill();
                return;
            }

            if (!SystemLocator::timerLocator->IsRunning(shield.cooldown)) {

                shield.hp--;
                SystemLocator::timerLocator->Start(0.5f, shield.cooldown);
            }

            projectile.combat.Kill();
        }
    }
}

void CollisionSystem::CheckPowerUps() {

    auto& powerups = SystemLocator::entityLocator->GetPowerUps();

    if (powerups.empty()) return;

    Player* player = SystemLocator::entityLocator->GetPlayer();

    for (auto& powerup : powerups) {

        if (CheckCollisionRecs(powerup.hitbox, player->GetHitBox())) {

            SystemLocator::scoreLocator->ApplyPowerUp(powerup.type);
            powerup.alive = false;
        }
    }
}

void CollisionSystem::CheckExplosions() {


    auto& explosions = SystemLocator::entityLocator->GetExplosions();
    auto& enemies = SystemLocator::entityLocator->GetEnemies();

    if (explosions.empty() || enemies.empty()) return;

    for (auto& explosion : explosions) {

        for (auto& enemy : enemies) {


            if (CheckCollisionCircleRec(explosion.center, explosion.radius, enemy.combat.hitbox)) {

                explosion.active = false;
                enemy.combat.Kill();
            }

        }
    }


}

