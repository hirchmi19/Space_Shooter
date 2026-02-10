//
// Created by Michael Hirsch on 06.02.26.
//

#include "RenderSystem.h"

#include "raylib.h"
#include "../../Game/GameWorld.h"

RenderSystem::RenderSystem() : IGameSystem(GameSystemID::RENDERER_SYSTEM) {}


void RenderSystem::Run(GameWorld& world) {

    RenderBackground(world);
    RenderPlayer(world);
    RenderProjectiles(world);
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
        world.GetPlayer().GetSize().x * RenderConstants::PLAYER_SCALING,
        world.GetPlayer().GetSize().y * RenderConstants::PLAYER_SCALING
    };

    if (playerSpeed > 0) playerSprite = world.GetSprite(SpriteID::PLAYER_SHIP_RIGHT);
    if (playerSpeed < 0) playerSprite = world.GetSprite(SpriteID::PLAYER_SHIP_LEFT);

    DrawTexturePro(playerTexture, playerSprite.src, dest, Vector2{0, 0}, 0.0f, WHITE);
    DrawRectangleLinesEx(world.GetPlayer().GetHitBox(), 1.0f, PINK);
}

void RenderSystem::RenderProjectiles(GameWorld& world) const  {


    auto& projectiles = world.GetProjectiles();

    for (auto& projectile : projectiles ) {

        const auto& texture = world.GetTexture(TextureID::ENEMY_SHIP_CANVAS);
        const auto& sprite = *(projectile.render.sprites[0]);

        const Rectangle dest = {
            projectile.movement.position.x,
            projectile.movement.position.y,
            projectile.render.size.x * RenderConstants::PROJECTILE_SCALING,
            projectile.render.size.y * RenderConstants::PROJECTILE_SCALING};

        DrawTexturePro(texture, sprite.src, dest, { 0, 0 }, 0.0f, WHITE);
       // DrawRectangleLinesEx(projectile.combat.hitbox, 1.0f, PINK);
    }

}
