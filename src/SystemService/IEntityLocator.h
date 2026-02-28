//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include "raylib.h"
#include "Entities/EntityType.h"
#include "Systems/Entities/SpawnType.h"


struct IEntityLocator {

    virtual ~IEntityLocator() = default;

    virtual void RequestEntitySpawn(const EntityType &eType, const SpawnType& sType, const Vector2 &pos) = 0;
    virtual Player* getPlayer() const = 0;
    virtual std::vector<Enemy>& GetEnemies() = 0;
    virtual std::vector<Projectile>& GetProjectiles() = 0;

};

