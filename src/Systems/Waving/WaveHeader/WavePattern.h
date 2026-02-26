//
// Created by Michael Hirsch on 12.02.26.
//

#pragma once

enum class WaveType : uint32_t;
enum class DiveType : uint32_t;

struct WavePattern {

    WaveType type;
    uint32_t numberOfDives;
    std::vector<int> groupSizes;
    std::vector<DiveType> dives;

};
