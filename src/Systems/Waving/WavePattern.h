//
// Created by Michael Hirsch on 12.02.26.
//

#pragma once
#include <cstdint>

#include "WaveType.h"

struct WavePattern {

    WaveType type;
    int numberOfDives;
    std::vector<int> groupSizes;
    std::vector<DiveType> dives;

};
