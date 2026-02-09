//
// Created by Michael Hirsch on 08.02.26.
//

#include "BackgroundSystem.h"

#include "../../Constants/GameConstants.h"


BackgroundSystem::BackgroundSystem() : IGameSystem(GameSystemID::BACKGROUND_SYSTEM){

    InitStars();
}

void BackgroundSystem::Run(GameWorld& world) {


    for (int i = 0; i < stars.size(); ++i) {

        stars[i].position.y++;

        if (stars[i].position.y >= GameConstants::SCREEN_HEIGHT) { RespawnStar(i);}
    }
}

void BackgroundSystem::Render() const {


    for (const auto& star : stars) {

        DrawPixel(static_cast<uint32_t>(star.position.x), static_cast<uint32_t>(star.position.y),
            Color {star.colorValue, star.colorValue, star.colorValue, 255});
    }
}


//--------------------------------------------------------------------------


void BackgroundSystem::InitStars() {


    for (int i = 0; i < stars.size(); ++i) {

        Star s;
        s.position.x = static_cast<float>(GetRandomValue(0, GameConstants::SCREEN_WIDTH));
        s.position.y = static_cast<float>(GetRandomValue(0, GameConstants::SCREEN_HEIGHT));
        s.colorValue = static_cast<unsigned char>(GetRandomValue(120, 255));
        stars[i] = s;
    }
}


void BackgroundSystem::RespawnStar(const size_t index) {

    stars[index].position.x = static_cast<float>(GetRandomValue(0, GetScreenWidth()));
    stars[index].position.y = 0.0f;
}
