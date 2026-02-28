//
// Created by Michael Hirsch on 26.02.26.
//

#include "EntitySystem.h"

#include <random>
#include <ranges>

#include "SpawnCommand.h"
#include "Constants/GameWorldConstants.h"
#include "Constants/WaveConstants.h"
#include "Systems/Assets/AssetSystem.h"
#include "SystemService/SystemLocator.h"

EntitySystem::EntitySystem() : IGameSystem(GameSystemID::ENTITY_SYSTEM, "ENTITY_SYSTEM"){}

void EntitySystem::Run(GameWorld &world) {

    if (!spawnCmds.empty()) SpawnEntities();
    if (!projectilesToRemove.empty() || !enemiesToRemove.empty()) KillEntities();
}

void EntitySystem::Init() {

    enemies.reserve(WaveConstants::NUMBER_OF_ENEMIES);
    projectiles.reserve(25);

    player = std::make_unique<Player>(
        GameWorldConstants::playerSpawn,
        GameWorldConstants::playerSize,
        SystemLocator::assetLocator->GetPlayerSprites());
}

void EntitySystem::HandleInputs() const {

    player->HandleInput();
}

void EntitySystem::RequestEntitySpawn(const EntityType &eType, const SpawnType &sType, const Vector2 &pos) {

    spawnCmds.emplace_back(eType, sType, pos);
}


//--------------------------------------------------------------------------

void EntitySystem::SpawnEntities() {

    for (const auto& cmd : spawnCmds) {

        if (cmd.sType == SpawnType::ENEMY) SpawnEnemy(cmd.eType, cmd.spawnPos);
        if (cmd.sType == SpawnType::PROJECTILE) SpawnProjectile(cmd.eType, cmd.spawnPos);

    }
}

void EntitySystem::SpawnEnemy(const EntityType &eType, const Vector2 &pos) {
    const auto& sprites =
        SystemLocator::assetLocator->GetEnemySprites(eType);

    const Vector2 size = {sprites[0]->src.width, sprites[0]->src.height};

    enemies.emplace_back(RenderComponent{sprites, size},
        CombatComponent{1,
            Rectangle{pos.x, pos.y,
            size.x * RenderConstants::ENEMY_SCALING,
            size.y * RenderConstants::ENEMY_SCALING},

            SystemLocator::scoreLocator->GetEnemyScore(eType)});

}

void EntitySystem::SpawnProjectile(const EntityType& eType, const Vector2 &pos) {

    const auto& projectileSprite = SystemLocator::assetLocator->GetProjectileSprite(eType);
    const int speed = eType == EntityType::PLAYER_PROJECTILE ? -1 : 1;
    bool isPlayerProjectile = eType == EntityType::PLAYER_PROJECTILE;

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

    FindDeadEntities();
    KillEnemies();
    KillProjectiles();
}

void EntitySystem::KillEnemies() {



    for (const auto& enemy : std::ranges::reverse_view(enemiesToRemove)) {

        enemies.erase(enemies.begin() + static_cast<long>(enemy));
    }

    enemiesToRemove.clear();
}

void EntitySystem::KillProjectiles() {

    for (const auto& projectile : std::ranges::reverse_view(projectilesToRemove)) {

        projectiles.erase(projectiles.begin() + static_cast<long>(projectile));
    }

    projectilesToRemove.clear();
}

void EntitySystem::FindDeadEntities() {

    FindDeadEnemies();
    FindDeadProjectiles();
}

void EntitySystem::FindDeadEnemies() {

    for (size_t i = 0; i < enemies.size(); ++i) {

        Enemy& enemy = enemies[i];
        if (enemy.combat.IsAlive()) continue;
        enemiesToRemove.push_back(i);
        SystemLocator::scoreLocator->AddHighScore(enemy.combat.score);
    }
}

void EntitySystem::FindDeadProjectiles() {

    for (size_t i = 0; i < projectiles.size(); ++i) {

        Projectile& projectile = projectiles[i];
        if (projectile.combat.IsAlive()) continue;
        projectilesToRemove.push_back(i);
    }
}

void EntitySystem::ClearEntities() {

    enemies.clear();
    projectiles.clear();
    projectilesToRemove.clear();
    enemiesToRemove.clear();
}








