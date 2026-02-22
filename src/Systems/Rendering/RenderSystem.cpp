//
// Created by Michael Hirsch on 06.02.26.
//

#include "RenderSystem.h"

#include <iostream>
#include "raylib.h"
#include "../../Game/GameWorld.h"

RenderSystem::RenderSystem() : IGameSystem(GameSystemID::RENDERER_SYSTEM) {}


void RenderSystem::Run(GameWorld& world) {

    RenderGameState(world, world.GetGameState());
}

//--------------------------------------------------------------------------

void RenderSystem::RenderGameState(GameWorld &world, const GameState state) const {

    std::string text;

    RenderBackground(world);

    switch (state) {

        case GameState::GAME_OVER:
            RenderGameOver(world);
            break;

        case GameState::IN_GAME:
            RenderPlayer(world);
            RenderEnemies(world);
            RenderProjectiles(world);
            break;

        case GameState::BEGIN_WAVE:

            text = "WAVE " + std::to_string(world.GetWaveCounter());
            RenderWaveTransition(world,text);
            break;


        case GameState::END_WAVE:

            text = "WAVE COMPLETED";
            RenderWaveTransition(world, text);
            break;
    }
}

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

    std::string caption = "GAME OVER";
    std::string subCaption = "PRESS `ENTER` TO RESTART";

    const Vector2 captionWidth = MeasureTextEx(world.GetFont(), caption.c_str(), RenderConstants::GAME_OVER_CAPTION_SIZE, RenderConstants::SPACING);
    const Vector2 subCaptionWidth = MeasureTextEx(world.GetFont(), subCaption.c_str(), RenderConstants::GAME_OVER_SUBCAPTION_SIZE, RenderConstants::SPACING);

    const Vector2 captionPos = {GameConstants::SCREEN_ORIGIN.x - captionWidth.x / 2, GameConstants::SCREEN_HEIGHT * .2f};
    const Vector2 subCaptionPos = {GameConstants::SCREEN_ORIGIN.x - subCaptionWidth.x / 2, captionPos.y + 225};

    DrawTextEx(world.GetFont(), caption.c_str(), captionPos, RenderConstants::GAME_OVER_CAPTION_SIZE, RenderConstants::SPACING, RED);
    DrawTextEx(world.GetFont(), subCaption.c_str(), subCaptionPos, RenderConstants::GAME_OVER_SUBCAPTION_SIZE, RenderConstants::SPACING, WHITE);
}

void RenderSystem::RenderWaveTransition(const GameWorld &world, std::string caption) const {

    const Vector2 captionWidth = MeasureTextEx(world.GetFont(), caption.c_str(), RenderConstants::WAVE_TRANSITION_CAPTION_SIZE, RenderConstants::SPACING);
    const Vector2 captionPos = {(GameConstants::SCREEN_ORIGIN.x - captionWidth.x / 2) + 30, GameConstants::SCREEN_HEIGHT * .22f};

    DrawTextEx(world.GetFont(), caption.c_str(), captionPos, RenderConstants::WAVE_TRANSITION_CAPTION_SIZE, RenderConstants::SPACING, WHITE);
}
