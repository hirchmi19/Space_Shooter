//
// Created by Michael Hirsch on 27.02.26.
//

#pragma once
#include "raylib.h"
#include "SpawnType.h"
#include "../../Entities/Enemies/EnemyType.h"
#include "Entities/EntityType.h"


struct SpawnEnemyCommand {

    EnemyType eType;
    Vector2 spawnPos;

};
