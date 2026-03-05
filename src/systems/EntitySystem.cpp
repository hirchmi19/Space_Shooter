//
// Created by Michael Hirsch on 26.02.26.
//

#include "systems/entity/EntitySystem.h"

#include <random>
#include <ranges>

#include "constants/GameWorldConstants.h"
#include "constants/MovementConstants.h"
#include "constants/WaveConstants.h"
#include "systems/assets/AssetSystem.h"
#include "locators/SystemLocator.h"

EntitySystem::EntitySystem() : IGameSystem(GameSystemID::ENTITY_SYSTEM, "ENTITY_SYSTEM") {}

void EntitySystem::Run() {

    FindDeadEntities();
    KillEntities();
}

void EntitySystem::Init() {

    enemies.reserve(WaveConstants::NUMBER_OF_ENEMIES);
    projectiles.reserve(25);

    const auto& shootTimer  = SystemLocator::timerLocator->CreateTimer(.0f, false); // create cooldown timer for player
    const auto& dashTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);
    const auto& playerSprites   = SystemLocator::assetLocator->GetPlayerSprites();

    player = std::make_unique<Player>(
       GameWorldConstants::playerSpawn,
       GameWorldConstants::playerSize,
       playerSprites, shootTimer, dashTimer);


    constexpr Vector2 shieldSpawnPos{
        GameWorldConstants::playerSpawnX +
            (GameWorldConstants::playerSize.x * RenderConstants::PLAYER_SCALING) / 2 -
                (GameWorldConstants::shieldSize.x * RenderConstants::SHIELD_SCALING) / 2,
        GameWorldConstants::playerSpawnY +
            (GameWorldConstants::playerSize.y * RenderConstants::PLAYER_SCALING) / 2 -
                (GameWorldConstants::shieldSize.y * RenderConstants::SHIELD_SCALING) / 2
    };

    shield = {shieldSpawnPos,
        {SystemLocator::assetLocator->GetShieldSprite(), GameWorldConstants::shieldSize},
        {
            shieldSpawnPos.x,
            shieldSpawnPos.y,
            GameWorldConstants::shieldSize.x * RenderConstants::SHIELD_SCALING,
            GameWorldConstants::shieldSize.y * RenderConstants::SHIELD_SCALING }
    };

}

void EntitySystem::HandleInputs() const {

    player->Run();
}


//--------------------------------------------------------------------------

void EntitySystem::SpawnEnemy(const EnemyType &eType, const Vector2 &spawnPos) {

    const auto& sprites =
        SystemLocator::assetLocator->GetEnemySprites(eType);

    const Vector2 size = {sprites[0]->src.width, sprites[0]->src.height};

    enemies.emplace_back(RenderComponent{sprites, size},
        CombatComponent{1,
            Rectangle{spawnPos.x, spawnPos.y,
            size.x * RenderConstants::ENEMY_SCALING,
            size.y * RenderConstants::ENEMY_SCALING},

            SystemLocator::scoreLocator->GetEnemyScore(eType)}
            );
}

void EntitySystem::SpawnProjectile(const ProjectileType &pType, const Vector2 &pos, bool isPlayerProjectile) {

    const auto& projectileSprite = SystemLocator::assetLocator->GetProjectileSprite(pType);
    const int dir = isPlayerProjectile ? -1 : 1;

    const Vector2 projectileSize {projectileSprite[0]->src.width,projectileSprite[0]->src.height};
    const float playerWidth = player->GetSize().x * RenderConstants::ENEMY_SCALING;
    const Vector2 spawnPosition {pos.x + (playerWidth * 0.5f) - (projectileSize.x * 0.5f),pos.y};

    projectiles.emplace_back(
        Movement1D{spawnPosition, dir, MovementConstants::PROJECTILE_SPEED},
        RenderComponent{projectileSprite, projectileSize},
        CombatComponent{1,Rectangle{spawnPosition.x,spawnPosition.y,
                projectileSize.x * RenderConstants::PROJECTILE_SCALING,
                projectileSize.y * RenderConstants::PROJECTILE_SCALING}}, isPlayerProjectile);

}

void EntitySystem::SpawnPowerUp(const PowerUpType type, const Vector2 &spawnPos) {

    const auto& sprite = SystemLocator::assetLocator->GetPowerUpSprite(type);
    constexpr int dir = 1;
    const Vector2 size = {sprite[0]->src.width,sprite[0]->src.height};

    powerUps.emplace_back(Movement1D{spawnPos, dir, MovementConstants::POWER_UP_SPEED},
        RenderComponent{sprite, size},
        Rectangle{spawnPos.x, spawnPos.y,
            RenderConstants::POWER_UP_SCALING * size.x,
            RenderConstants::POWER_UP_SCALING * size.y
        }, true);
}

void EntitySystem::KillEntities() {

    KillEnemies();
    KillProjectiles();
    KillPowerUps();
}

void EntitySystem::KillEnemies() {

    for (const auto& enemy : std::ranges::reverse_view(deadEnemies)) {

        enemies.erase(enemies.begin() + static_cast<long>(enemy));
    }

    deadEnemies.clear();
}

void EntitySystem::KillProjectiles() {

    for (const auto& projectile : std::ranges::reverse_view(deadProjectiles)) {

        projectiles.erase(projectiles.begin() + static_cast<long>(projectile));
    }

    deadProjectiles.clear();
}

void EntitySystem::KillPowerUps() {

    for (const auto& powerUp : std::ranges::reverse_view(deadPowerUps)) {

        powerUps.erase(powerUps.begin() + static_cast<long>(powerUp));
    }

    deadPowerUps.clear();
}

void EntitySystem::FindDeadEntities() {

    FindDeadEnemies();
    FindDeadProjectiles();
    FindDeadPowerUps();
}

void EntitySystem::FindDeadEnemies() {

    for (size_t i = 0; i < enemies.size(); ++i) {

        Enemy& enemy = enemies[i];
        if (enemy.combat.IsAlive()) continue;
        RequestEntityRemoval(EntityType::ENEMY, i);
        SystemLocator::scoreLocator->AddHighScore(enemy.combat.score, enemy.wave.worldPosition);
    }
}

void EntitySystem::FindDeadProjectiles() {

    for (size_t i = 0; i < projectiles.size(); ++i) {

        Projectile& projectile = projectiles[i];
        if (projectile.combat.IsAlive()) continue;
        RequestEntityRemoval(EntityType::PROJECTILE, i);
    }
}

void EntitySystem::FindDeadPowerUps() {

    for (size_t i = 0; i < powerUps.size(); ++i) {

        PowerUp& pup = powerUps[i];
        if (pup.alive) continue;
        RequestEntityRemoval(EntityType::POWER_UP, i);
    }
}

void EntitySystem::RequestEntityRemoval(const EntityType &eType, const size_t value) {

    auto& removalQ = GetRemovalQ(eType);
    const auto index = std::ranges::lower_bound(removalQ, value);
    removalQ.insert(index, value);

}

std::vector<size_t>& EntitySystem::GetRemovalQ(const EntityType &type) {

    if (type == EntityType::PROJECTILE) return deadProjectiles;
    if (type == EntityType::ENEMY) return deadEnemies;
    if (type == EntityType::POWER_UP) return deadPowerUps;

}

void EntitySystem::ClearEntities() {

    enemies.clear();
    projectiles.clear();
    deadProjectiles.clear();
    deadEnemies.clear();
    deadPowerUps.clear();
    powerUps.clear();
}








