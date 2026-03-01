//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include "raylib.h"
#include "Entities/Enemy.h"
#include "Entities/EnemyType.h"
#include "Entities/Projectile.h"
#include "Entities/ProjectileType.h"
#include "Entities/Player/Player.h"

struct IEntityLocator {

    virtual ~IEntityLocator() = default;

    virtual void RequestEnemySpawn(const EnemyType& eType, const Vector2& pos) = 0;
    virtual void RequestProjectileSpawn(const ProjectileType& pType, const Vector2& pos, bool isPlayerProjectile) = 0;
    virtual void RequestEntityRemoval(const EntityType&, size_t value) = 0;
    virtual Player* GetPlayer() const = 0;
    virtual std::vector<Enemy>& GetEnemies() = 0;
    virtual std::vector<Projectile>& GetProjectiles() = 0;

};

