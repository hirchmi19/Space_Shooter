//
// Created by Michael Hirsch on 06.02.26.
//

#include "RenderSystem.h"

#include <iostream>
#include "raylib.h"
#include "../../Game/GameWorld.h"

RenderSystem::RenderSystem() : IGameSystem(GameSystemID::RENDERER_SYSTEM) {}


void RenderSystem::Run(GameWorld& world) {

    RenderBackground(world);

    if (world.GetGameState() == GameState::GAME_OVER) {

        RenderGameOver(world);
        return;
    }

    RenderPlayer(world);
    RenderEnemies(world);
    RenderProjectiles(world);
}

//--------------------------------------------------------------------------



void RenderSystem::RenderBackground(const GameWorld& world) const {

    world.RenderBackground();
}

/**
 * Renders all enemies and shift their sprites
 * \param world
 */
void RenderSystem::RenderEnemies(GameWorld& world) const {

    const auto& enemies = world.GetEnemies();
    const double time = GetTime();
    const size_t spriteToDraw = static_cast<size_t>(time * 4) % 2; // flip between sprites to simulate animation

    for (const auto& enemy : enemies) {

        if (enemy.wave.state == WaveState::OUT_FORMATION) continue;

        const auto& texture = world.GetTexture(TextureID::ENEMY_SHIP_CANVAS);
        const auto& sprite = *(enemy.render.sprites[spriteToDraw]);

        const Rectangle dest = {
            enemy.wave.worldPosition.x,
            enemy.wave.worldPosition.y,
            enemy.render.size.x * RenderConstants::ENEMY_SCALING,
            enemy.render.size.y * RenderConstants::ENEMY_SCALING};

        DrawTexturePro(texture, sprite.src, dest, { 0, 0 }, 0.0f, WHITE);
        DrawRectangleLinesEx(enemy.combat.hitbox, 1.0f, PINK);
    }
}


/**
 * Renders the player
 * \param world
 */
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

/**
 * Renders all projectiles
 * \param world
 */
void RenderSystem::RenderProjectiles(GameWorld& world) const  {

    const auto& projectiles = world.GetProjectiles();

    for (const auto& projectile : projectiles ) {

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

void RenderSystem::RenderGameOver(const GameWorld &world) const {

    if (world.GetGameState() != GameState::GAME_OVER) return;

    const char* caption = "GAME OVER";
    const char* subCaption = "PRESS `ENTER` TO RESTART";

    const Vector2 captionWidth = MeasureTextEx(world.GetFont(), caption, RenderConstants::CAPTION_SIZE, RenderConstants::SPACING);
    const Vector2 subCaptionWidth = MeasureTextEx(world.GetFont(), subCaption, RenderConstants::SUBCAPTION_SIZE, RenderConstants::SPACING);

    const Vector2 captionPos = {GameConstants::SCREEN_ORIGIN.x - captionWidth.x / 2, GameConstants::SCREEN_HEIGHT * .2f};
    const Vector2 subCaptionPos = {GameConstants::SCREEN_ORIGIN.x - subCaptionWidth.x / 2, captionPos.y + 225};

    DrawTextEx(world.GetFont(), caption, captionPos, RenderConstants::CAPTION_SIZE, RenderConstants::SPACING, RED);
    DrawTextEx(world.GetFont(), subCaption, subCaptionPos, RenderConstants::SUBCAPTION_SIZE, RenderConstants::SPACING, WHITE);
}
