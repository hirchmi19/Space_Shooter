//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once
#include "DivingGroup.h"
#include "raylib.h"
#include "../../Entities/EnemyID.h"

struct Enemy;

struct FormationSlot {

    Vector2 position;
    DivingGroup group;
    EnemyID id = EnemyID::COUNT;
    uint32_t slotIndex;

};
