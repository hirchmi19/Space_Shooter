//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once
#include <cstdint>

enum class WavePhase : uint32_t {


    NONE,
    INITIALIZE,
    BUILDING_FORMATION,
    FORMATION_ON,
    FORMATION_OFF,
    SOLO_ATTACK,
    SWARM_OUT,
    COUNT
};
