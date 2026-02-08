//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

#include "CombatData.h"
#include "Movement2D.h"
#include "RenderData.h"

struct Enemy {

   Movement2D movement;
   RenderData render;
   CombatData combat;

};
