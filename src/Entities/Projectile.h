//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once
#include "../Constants/MovementConstants.h"
#include "Components/Movement1D.h"
#include "Components/CombatComponent.h"
#include "Components/RenderComponent.h"

struct Projectile {

    Movement1D movement;
    RenderComponent render;
    CombatComponent combat;

};

