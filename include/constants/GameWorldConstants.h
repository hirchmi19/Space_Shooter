//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include <cstdint>

#include "GameConstants.h"
#include "raylib.h"

namespace GameWorldConstants {

    constexpr float playerSpawnX = GameConstants::SCREEN_WIDTH / 2.0f - 8; // minus texture width
    constexpr float playerSpawnY = GameConstants::SCREEN_HEIGHT - 100.0f; // minus an offset so player isn't at the bottom

    constexpr Vector2 playerSpawn = {playerSpawnX, playerSpawnY};
    constexpr Vector2 playerSize = {8, 7}; // player texture value

}
