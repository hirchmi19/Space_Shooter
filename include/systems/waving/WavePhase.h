//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once
#include <cstdint>

enum class WavePhase {


    NONE,
    INITIALIZE,
    BUILDING_FORMATION,
    FORMATION_ON,
    FORMATION_OFF,
    SOLO_ATTACK,
    READY_FOR_SWARM,
    SWARM_ATTACK,
    COUNT
};
