//
// Created by Michael Hirsch on 14.02.26.
//
#pragma once
#include <cstdint>

namespace WaveConstants {

    constexpr std::array ROW_COUNTS = {4, 8, 8, 12, 12};
    constexpr float HORIZONTAL_SPACING = 60.0f;
    constexpr float VERTICAL_SPACING   = 65.0f;
    static int WAVE_COUNTER = 0;
    constexpr uint32_t NUMBER_OF_ENEMIES = 44;

}
