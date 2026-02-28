//
// Created by Michael Hirsch on 27.02.26.
//

#pragma once
#include "raylib.h"
#include "SpawnType.h"
#include "Entities/EntityType.h"


struct SpawnCommand {

    EntityType eType;
    SpawnType sType;
    Vector2 spawnPos;

};
