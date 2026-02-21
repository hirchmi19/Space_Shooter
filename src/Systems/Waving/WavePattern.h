//
// Created by Michael Hirsch on 12.02.26.
//

#pragma once

#include "WaveType.h"

struct WavePattern {

    WaveType type;
    uint32_t numberOfDives;
    std::vector<int> groupSizes;
    std::vector<DiveType> dives;

};
