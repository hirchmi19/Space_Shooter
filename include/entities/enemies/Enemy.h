//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

#include "../Entity.h"
#include "../../components/CombatComponent.h"
#include "../../components/WaveComponent.h"

struct Enemy : Entity {

   CombatComponent combat;
   WaveComponent wave;

};
