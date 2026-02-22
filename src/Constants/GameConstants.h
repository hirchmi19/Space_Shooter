//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include <cstdint>

namespace   GameConstants {

  inline constexpr uint32_t SCREEN_WIDTH = 1200;
  inline constexpr uint32_t SCREEN_HEIGHT = 900;
  inline constexpr uint32_t TARGET_FPS = 60;
  inline constexpr Vector2 SCREEN_ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
  inline constexpr float UPS = 60;
};