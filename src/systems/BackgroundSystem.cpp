//
// Created by Michael Hirsch on 08.02.26.
//

#include "systems/rendering/BackgroundSystem.h"
#include "constants/GameConstants.h"
#include "game/GameWorld.h"
#include "locators/SystemLocator.h"

BackgroundSystem::BackgroundSystem()
    : IGameSystem(GameSystemID::BACKGROUND_SYSTEM, "BACKGROUND_SYSTEM") {}

void BackgroundSystem::Run() {

  for (size_t i = 0; i < stars.size(); ++i) {

    speed =
        SystemLocator::entityLocator->GetPlayer()->IsInFlowState() ? 15.f : 1.f;
    stars[i].position.y += speed;

    if (stars[i].position.y >= GameConstants::SCREEN_HEIGHT)
      RespawnStar(i);
  }
}

/**
 * Renders all stars
 */
void BackgroundSystem::Render() {

  for (const auto &star : stars) {

    DrawPixel(static_cast<int>(star.position.x),
              static_cast<int>(star.position.y),
              Color{star.colorValue, star.colorValue, star.colorValue, 255});
  }
}

//--------------------------------------------------------------------------

/**
 * Spawns all initial stars in the background
 */
void BackgroundSystem::Init() {

  for (auto &star : stars) {

    star.position.x =
        static_cast<float>(GetRandomValue(0, GameConstants::SCREEN_WIDTH));
    star.position.y =
        static_cast<float>(GetRandomValue(0, GameConstants::SCREEN_HEIGHT));
    star.colorValue = static_cast<unsigned char>(GetRandomValue(120, 255));

  }
}

/**
 * Respawns a star, after he leaves the screen space
 * \param index
 */
void BackgroundSystem::RespawnStar(const size_t index) {

  stars[index].position.x =
      static_cast<float>(GetRandomValue(0, GetScreenWidth()));
  stars[index].position.y = 0.0f;
}
