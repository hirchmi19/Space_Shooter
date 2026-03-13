//
// Created by Michael Hirsch on 26.02.26.
//

#include "systems/entity/EntitySystem.h"

#include <iostream>
#include <ostream>
#include <random>
#include <ranges>

#include "constants/GameWorldConstants.h"
#include "constants/MovementConstants.h"
#include "constants/WaveConstants.h"
#include "systems/assets/AssetSystem.h"
#include "locators/SystemLocator.h"

EntitySystem::EntitySystem() : IGameSystem(GameSystemID::ENTITY_SYSTEM, "ENTITY_SYSTEM") {}

void EntitySystem::Run() {

    if (shield.hp <= 0 ) player->DeactivateShield();

    FindDeadEntities(enemies, deadEnemies);
    FindDeadEntities(projectiles, deadProjectiles);
    FindDeadEntities(powerUps, deadPowerUps);
    FindDeadEntities(explosions, deadExplosions);

    HandleEnemyDeaths(); // IMPORTANT!: handle death enemy events before killing them
    KillEntities(enemies, deadEnemies);
    KillEntities(projectiles, deadProjectiles);
    KillEntities(powerUps, deadPowerUps);
    KillEntities(explosions, deadExplosions);
}

void EntitySystem::Init() {

    enemies.reserve(WaveConstants::NUMBER_OF_ENEMIES);
    projectiles.reserve(25);

    InitPlayer();
    InitShield();
}

void EntitySystem::InitPlayer() {

    const auto& shootTimer  = SystemLocator::timerLocator->CreateTimer(.0f, false);
    const auto& dashTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);
    const auto& prjectileTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);
    const auto& playerSprites   = SystemLocator::assetLocator->GetPlayerSprite();

    player = std::make_unique<Player>(
       GameWorldConstants::playerSpawn,
       GameWorldConstants::playerSize,
       playerSprites, shootTimer, dashTimer, prjectileTimer);

}

void EntitySystem::InitShield() {

    constexpr Vector2 playerCenter = {
        GameWorldConstants::playerSpawnX + (GameWorldConstants::playerSize.x * RenderConstants::PLAYER_SCALING) / 2,
        GameWorldConstants::playerSpawnY + (GameWorldConstants::playerSize.y * RenderConstants::PLAYER_SCALING) / 2
    };

    constexpr Vector2 spawnPos = {
        playerCenter.x - (GameWorldConstants::shieldSize.x * RenderConstants::SHIELD_SCALING) / 2,
        playerCenter.y - (GameWorldConstants::shieldSize.y * RenderConstants::SHIELD_SCALING) / 2
    };

    shield.position = spawnPos;
    shield.render   = {SystemLocator::assetLocator->GetEffectSprite(EffectID::SHIELD), GameWorldConstants::shieldSize};
    shield.hitbox   = {
        spawnPos.x, spawnPos.y,
        GameWorldConstants::shieldSize.x * RenderConstants::SHIELD_SCALING,
        GameWorldConstants::shieldSize.y * RenderConstants::SHIELD_SCALING};

    shield.cooldown = SystemLocator::timerLocator->CreateTimer(0.0f, false);
}

void EntitySystem::HandleInputs() const {

    player->Run();
}

void EntitySystem::LvlProjectiles() {

    if (projctileHp >= 3) return;
    projctileHp++;
}

void EntitySystem::SpawnEnemy(const EnemyType &eType, const Vector2 &spawnPos) {

    const auto& sprites = SystemLocator::assetLocator->GetEnemySprites(eType);
    const Vector2 size = {sprites[0]->src.width, sprites[0]->src.height};

    Enemy enemy;
    enemy.position = spawnPos;
    enemy.render = {sprites, size};
    enemy.hitbox = {spawnPos.x, spawnPos.y, size.x * RenderConstants::ENEMY_SCALING, size.y * RenderConstants::ENEMY_SCALING};
    enemy.combat = CombatComponent{1, SystemLocator::scoreLocator->GetEnemyScore(eType)};
    enemies.emplace_back(enemy);
}

void EntitySystem::SpawnProjectile(const ProjectileType &pType, const Vector2 &pos, bool isPlayerProjectile) {

    const auto& sprite = SystemLocator::assetLocator->GetProjectileSprites(pType);
    const int dir = isPlayerProjectile ? -1 : 1;
    const Vector2 size = {sprite[0]->src.width, sprite[0]->src.height};
    const float playerWidth = player->GetSize().x * RenderConstants::PLAYER_SCALING;
    const Vector2 spawnPos = {pos.x + (playerWidth * 0.5f) - (size.x * 0.5f), pos.y};

    Projectile p;
    p.position = spawnPos;
    p.render = {sprite, size};
    p.hitbox = {spawnPos.x, spawnPos.y, size.x * RenderConstants::PROJECTILE_SCALING, size.y * RenderConstants::PROJECTILE_SCALING};
    p.movement = {dir, MovementConstants::PROJECTILE_SPEED};
    p.combat = CombatComponent{projctileHp};
    p.isPlayerProjectile = isPlayerProjectile;
    p.type = pType;
    projectiles.emplace_back(p);

}

void EntitySystem::SpawnPowerUp(const PowerUpType type, const Vector2 &spawnPos) {

    if (type == PowerUpType::NONE) return;

    const auto& sprite = SystemLocator::assetLocator->GetPowerUpIcon(type);
    const Vector2 size = {sprite[0]->src.width, sprite[0]->src.height};

    PowerUp p;
    p.position = spawnPos;
    p.render = {sprite, size};
    p.hitbox = {spawnPos.x, spawnPos.y, size.x * RenderConstants::POWER_UP_SCALING, size.y * RenderConstants::POWER_UP_SCALING};
    p.movement = { 1, MovementConstants::POWER_UP_SPEED};
    p.type = type;
    powerUps.emplace_back(p);
}

void EntitySystem::SpawnExplosion(const Vector2 &pos) {

    const std::vector<const Sprite*>& sprite = {SystemLocator::assetLocator->GetEffectSprite(EffectID::EXPLOSION)};
    const Vector2 size = {sprite[0]->src.width,sprite[0]->src.height};

    Explosion e;
    e.position = pos;
    e.render = {sprite, size};
    e.hitbox = {pos.x, pos.y, size.x * RenderConstants::EXPLOSION_SCALING, size.y * RenderConstants::EXPLOSION_SCALING};
    e.lifetime = SystemLocator::timerLocator->CreateTimer(0.5f, true);
    explosions.emplace_back(e);

}

//----------------------------------------------------------------------------------------------------

/**
 * Handles the death enemy event
 */
void EntitySystem::HandleEnemyDeaths() const {

    for (const auto& i : deadEnemies) {

        const auto& e = enemies.at(i);
        SystemLocator::scoreLocator->AddScore(e.combat.score, e.position);

    }

}

/**
 * Adds the indice of an dead entity to its according removal queue
 * \param removalQ
 * \param index
 */
void EntitySystem::RequestEntityRemoval(std::vector<size_t> &removalQ, const size_t index) {

    const auto it = std::ranges::lower_bound(removalQ, index);
    removalQ.insert(it, index);

}

/**
 * Clears all entity collections
 */
void EntitySystem::ClearEntities() {

    enemies.clear();
    projectiles.clear();
    deadProjectiles.clear();
    deadEnemies.clear();
    deadPowerUps.clear();
    powerUps.clear();
    explosions.clear();
}








