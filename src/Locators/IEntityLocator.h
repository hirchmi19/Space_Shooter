//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once

#include "raylib.h"
#include "Entities/Entities.h"
#include "Systems/Entities/SpawnEnemyCommand.h"

struct IEntityLocator {

    virtual ~IEntityLocator() = default;

    virtual void SpawnProjectile(const ProjectileType &pType, const Vector2& pos, bool isPlayerProjectile) = 0;
    virtual void SpawnEnemy(const EnemyType &eCmd, const Vector2 &spawnPos) = 0;
    virtual void RequestEntityRemoval(const EntityType&, size_t value) = 0;
    virtual Player* GetPlayer() const = 0;
    virtual std::vector<Enemy>& GetEnemies() = 0;
    virtual std::vector<Projectile>& GetProjectiles() = 0;

};

