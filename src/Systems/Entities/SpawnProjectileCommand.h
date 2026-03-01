//
// Created by Michael Hirsch on 01.03.26.
//

#pragma once
#include "raylib.h"
#include "Entities/ProjectileType.h"

struct SpawnProjectileCommand {

    ProjectileType pType;
    Vector2 spawnPos;
    bool isPlayerProjectile;

};
