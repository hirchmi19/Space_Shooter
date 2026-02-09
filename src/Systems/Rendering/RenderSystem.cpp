//
// Created by Michael Hirsch on 06.02.26.
//

#include "RenderSystem.h"

#include "raylib.h"
#include "../../GameWorld.h"

RenderSystem::RenderSystem() : IGameSystem(GameSystemID::RENDERER_SYSTEM) {}


void RenderSystem::Run(GameWorld& world) {

    RenderBackground(world);
    RenderPlayer(world);
}

//--------------------------------------------------------------------------



void RenderSystem::RenderBackground(const GameWorld& world) const {

    world.RenderBackground();
}

void RenderSystem::RenderEnemies(const GameWorld& world) const {

}


void RenderSystem::RenderPlayer(const GameWorld& world) const {


    const auto& playerTexture = world.GetTexture(TextureID::PLAYER_SHIP_CANVAS);
    auto playerSprite = world.GetSprite(SpriteID::PLAYER_SHIP_MIDDLE);
    const auto& playerSpeed = world.GetPlayer().GetSpeed();
    const Rectangle dest = {
        world.GetPlayer().GetPosition().x,
        world.GetPlayer().GetPosition().y,
        world.GetPlayer().GetSize().x * PLAYER_SCALING,
        world.GetPlayer().GetSize().y * PLAYER_SCALING
    };

    if (playerSpeed > 0) playerSprite = world.GetSprite(SpriteID::PLAYER_SHIP_RIGHT);
    if (playerSpeed < 0) playerSprite = world.GetSprite(SpriteID::PLAYER_SHIP_LEFT);

    DrawTexturePro(playerTexture, playerSprite.src, dest, Vector2{0, 0}, 0.0f, WHITE);
}
