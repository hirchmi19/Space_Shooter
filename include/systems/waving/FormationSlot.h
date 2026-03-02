//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once
#include "DivingGroup.h"
#include "raylib.h"

enum class EnemyType;
struct DivingGroup;

struct Enemy;

struct FormationSlot {

    Vector2 position{};
    DivingGroup group{};
    EnemyType type{};
    int slotIndex = 0;

};
