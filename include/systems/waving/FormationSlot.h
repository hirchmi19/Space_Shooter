//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once
#include "raylib.h"

enum class EnemyType;
struct DivingGroup;

struct Enemy;

struct FormationSlot {

    Vector2 position;
    int groupID;
    EnemyType type;
    int gridIndex = 0;

};
