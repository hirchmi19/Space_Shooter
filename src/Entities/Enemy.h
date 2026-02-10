//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

#include "Components/CombatComponent.h"
#include "Components/Movement2D.h"
#include "Components/RenderComponent.h"

struct Enemy {

   Movement2D movement;
   RenderComponent render;
   CombatComponent combat;

};
