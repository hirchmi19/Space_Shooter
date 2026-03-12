//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

#include "../Entity.h"
#include "ProjectileType.h"
#include "../../components/Movement1D.h"
#include "../../components/CombatComponent.h"

struct Projectile : Entity{

    Movement1D movement;
    CombatComponent combat;
    bool isPlayerProjectile = true;
    ProjectileType type = ProjectileType::BASE_PLAYER;

};

