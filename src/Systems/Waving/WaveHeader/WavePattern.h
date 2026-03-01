//
// Created by Michael Hirsch on 12.02.26.
//

#pragma once

enum class WaveType;
enum class DiveType;

struct WavePattern {

    WaveType type;
    int numberOfDives;
    std::vector<int> groupSizes;
    std::vector<DiveType> dives;

};
