//
// Created by Michael Hirsch on 26.02.26.
//

#include "systems/entity/EntitySystem.h"

#include <random>
#include <ranges>

#include "constants/GameWorldConstants.h"
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

    const auto& timer = SystemLocator::timerLocator->CreateTimer(.0f, false); // create cooldown timer for player

    player = std::make_unique<Player>(
       GameWorldConstants::playerSpawn,
       GameWorldConstants::playerSize,
       SystemLocator::assetLocator->GetPlayerSprites(), timer);

}

void EntitySystem::HandleInputs() const {

    player->HandleInput();
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
    const int speed = isPlayerProjectile ? -1 : 1;

    const Vector2 projectileSize {projectileSprite[0]->src.width,projectileSprite[0]->src.height};
    const float playerWidth = player->GetSize().x * RenderConstants::ENEMY_SCALING;
    const Vector2 spawnPosition {pos.x + (playerWidth * 0.5f) - (projectileSize.x * 0.5f),pos.y};

    projectiles.emplace_back(
        Movement1D{spawnPosition, speed},
        RenderComponent{projectileSprite, projectileSize},
        CombatComponent{1,Rectangle{spawnPosition.x,spawnPosition.y,
                projectileSize.x * RenderConstants::PROJECTILE_SCALING,
                projectileSize.y * RenderConstants::PROJECTILE_SCALING}}, isPlayerProjectile);

}

void EntitySystem::KillEntities() {

    KillEnemies();
    KillProjectiles();
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

void EntitySystem::FindDeadEntities() {

    FindDeadEnemies();
    FindDeadProjectiles();
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

void EntitySystem::RequestEntityRemoval(const EntityType &eType, const size_t value) {

    auto& removalQ = eType == EntityType::ENEMY ? deadEnemies : deadProjectiles;
    const auto index = std::ranges::lower_bound(removalQ, value);
    removalQ.insert(index, value);

}

void EntitySystem::ClearEntities() {

    enemies.clear();
    projectiles.clear();
    deadProjectiles.clear();
    deadEnemies.clear();
}








