//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

#include "../../components/CombatComponent.h"
#include "../../components/RenderComponent.h"
#include "../../components/WaveComponent.h"

struct Enemy {

   RenderComponent render;
   CombatComponent combat;
   WaveComponent wave;

};
