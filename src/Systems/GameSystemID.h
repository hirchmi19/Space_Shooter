//
// Created by Michael Hirsch on 06.02.26.
//

#pragma once
#include <cstdint>

enum class GameSystemID : uint32_t {

    NONE,
    ASSET_SYSTEM,
    RENDERER_SYSTEM,
    SOUND_SYSTEM,
    COUNT
};
