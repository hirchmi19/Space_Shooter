//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once

#include "raylib.h"
#include "entities/Player.h"
#include "../entities/Entities.h"
#include "entities/enemies/EnemyType.h"
#include "../entities/Player.h"
#include "entities/Explosion.h"
#include "entities/PowerUpType.h"
#include "entities/PowerUp.h"
#include "entities/Shield.h"


struct IEntityLocator {

    virtual ~IEntityLocator() = default;

    virtual void SpawnProjectile(const ProjectileType &pType, const Vector2& pos, bool isPlayerProjectile) = 0;
    virtual void SpawnEnemy(const EnemyType &eCmd, const Vector2 &spawnPos) = 0;
    virtual void RequestEntityRemoval(std::vector<size_t> &removalQ, size_t index) = 0;
    virtual void SpawnPowerUp(PowerUpType type, const Vector2& spawnPos) = 0;
    virtual Player* GetPlayer() const = 0;
    virtual Shield& GetShield() = 0;
    virtual std::vector<Enemy>& GetEnemies() = 0;
    virtual std::vector<Projectile>& GetProjectiles() = 0;
    virtual std::vector<PowerUp>& GetPowerUps() = 0;
    virtual std::vector<Explosion>& GetExplosions() = 0;
    virtual void LvlProjectiles() = 0;
    virtual float GetProjectileHp() = 0;
    virtual void Reset() = 0;
    virtual void SpawnExplosion(const Vector2& pos) = 0;
};

