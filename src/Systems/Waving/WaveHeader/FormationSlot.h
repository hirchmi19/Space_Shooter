//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once
#include "DivingGroup.h"
#include "raylib.h"

enum class EnemyType : uint32_t;
struct DivingGroup;

struct Enemy;

struct FormationSlot {

    Vector2 position{};
    DivingGroup group{};
    EnemyType id{};
    uint32_t slotIndex = 0;

};
