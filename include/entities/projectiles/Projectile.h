//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

#include "../../components/Movement1D.h"
#include "../../components/CombatComponent.h"
#include "../../components/RenderComponent.h"

struct Projectile {

    Movement1D movement;
    RenderComponent render;
    CombatComponent combat;
    bool isPlayerProjectile = true;

};

