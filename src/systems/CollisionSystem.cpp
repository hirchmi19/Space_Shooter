//
// Created by Michael Hirsch on 21.02.26.
//


#include "components/WaveState.h"
#include "systems/collision/CollisionSystem.h"

#include <iostream>

#include "constants/TimerDurations.h"
#include "locators/SystemLocator.h"

CollisionSystem::CollisionSystem() : IGameSystem(GameSystemID::COLLISION_SYSTEM, "COLLISION_SYSTEM"){}

void CollisionSystem::Run() {

    CheckEnemiesProjectiles();
    CheckPlayerProjectiles();
    CheckPlayerEnemies();
    CheckPowerUps();
    CheckExplosions();
}

//--------------------------------------------------------------------------

void CollisionSystem::CheckEnemiesProjectiles() {


    auto& enemies     = SystemLocator::entityLocator->GetEnemies();
    auto& projectiles = SystemLocator::entityLocator->GetProjectiles();

    for (auto& enemy : enemies) {
        for (auto& projectile : projectiles) {

            if (!projectile.isPlayerProjectile) continue;

            //brodphase check -- ignore if cleary out of range
            //if (std::abs(enemy.position.y - projectile.position.y) > enemy.render.size.y) continue;

            if (!CheckCollisionRecs(enemy.hitbox, projectile.hitbox)) continue;

            const auto& powerUp = SystemLocator::scoreLocator->RollPowerUpDrop();
            SystemLocator::entityLocator->SpawnPowerUp(powerUp, enemy.position);
            enemy.isAlive = false; // enemy always dies

            if (projectile.type == ProjectileType::ARROW) {

                projectile.combat.TakeDamage(0.5f);
                if (!projectile.combat.IsAlive()) projectile.isAlive = false;
                continue;
            }

            if (projectile.type == ProjectileType::ROCKET)
                SystemLocator::entityLocator->SpawnExplosion(enemy.position);

            if (projectile.type != ProjectileType::GLAIVE) // Glaives melts through everything
                projectile.isAlive = false;
        }
    }
}

void CollisionSystem::CheckPlayerEnemies() {

    auto& enemies  = SystemLocator::entityLocator->GetEnemies();
    Player* player = SystemLocator::entityLocator->GetPlayer();
    auto& shield   = SystemLocator::entityLocator->GetShield();

    for (auto& enemy : enemies) {

        if (enemy.wave.state != WaveState::ATTACK) continue;
        if (!CheckCollisionRecs(enemy.hitbox, player->GetHitBox())) continue;
        if (std::abs(enemy.position.x - player->GetPosition().x) >
            player->GetSize().x * RenderConstants::PLAYER_SCALING) continue;

        if (!player->IsShieldActive()) {
            player->isAlive = false;
            return;
        }

        DamageShield(shield);
        enemy.isAlive = false;
    }
}

void CollisionSystem::CheckPlayerProjectiles() {

    auto& projectiles = SystemLocator::entityLocator->GetProjectiles();
    Player* player    = SystemLocator::entityLocator->GetPlayer();
    auto& shield      = SystemLocator::entityLocator->GetShield();

    for (auto& projectile : projectiles) {

        if (projectile.isPlayerProjectile) continue;
        if (projectile.position.y < player->GetPosition().y) continue;
        if (!CheckCollisionRecs(projectile.hitbox, player->GetHitBox())) continue;
        if (std::abs(projectile.position.x - player->GetPosition().x) > player->GetSize().x) continue;

        if (!player->IsShieldActive()) {
            player->isAlive = false;
            return;
        }

        DamageShield(shield);
        projectile.isAlive = false;
    }
}

void CollisionSystem::CheckPowerUps() {

    auto& powerups = SystemLocator::entityLocator->GetPowerUps();
    if (powerups.empty()) return;

    Player* player = SystemLocator::entityLocator->GetPlayer();

    for (auto& powerup : powerups) {

        if (!CheckCollisionRecs(powerup.hitbox, player->GetHitBox())) continue;
        SystemLocator::scoreLocator->ApplyPowerUp(powerup.type);
        powerup.isAlive = false;
    }
}

void CollisionSystem::CheckExplosions() {

    auto& explosions = SystemLocator::entityLocator->GetExplosions();
    auto& enemies    = SystemLocator::entityLocator->GetEnemies();

    if (explosions.empty() || enemies.empty()) return;

    for (auto& explosion : explosions) {

        if (!explosion.isAlive) continue;

        for (auto& enemy : enemies) {

            // broadphase check
            //if (std::abs(enemy.position.x - explosion.position.x) > explosion.render.size.x) continue;
            //if (std::abs(enemy.position.y - explosion.position.y) > explosion.render.size.y) continue;
            if (!CheckCollisionRecs(explosion.hitbox, enemy.hitbox)) continue;

            enemy.isAlive = false;
        }

       if (!SystemLocator::timerLocator->IsRunning(explosion.lifetime)) explosion.isAlive = false;
    }
}

void CollisionSystem::DamageShield(Shield& shield) {

    if (SystemLocator::timerLocator->IsRunning(shield.cooldown)) return;
    shield.hp--;
    SystemLocator::timerLocator->Start(TimerDurations::SHIELD_COOLDOWN, shield.cooldown);
}

