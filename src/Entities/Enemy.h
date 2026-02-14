//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

#include "Components/CombatComponent.h"
#include "Components/RenderComponent.h"
#include "Components/WaveComponent.h"

struct Enemy {

   RenderComponent render;
   CombatComponent combat;
   WaveComponent wave;

};
