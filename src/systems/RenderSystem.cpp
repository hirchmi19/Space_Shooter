//
// Created by Michael Hirsch on 06.02.26.
//

#include <iostream>
#include "raylib.h"
#include "systems/rendering/RenderSystem.h"
#include "constants/GameConstants.h"
#include "constants/ScoringConstants.h"
#include "constants/WaveConstants.h"
#include "locators/SystemLocator.h"
#include "systems/assets/Sprite.h"
#include "systems/assets/SpriteID.h"
#include "systems/assets/TextureID.h"
#include "utils/utils.h"

RenderSystem::RenderSystem() : IGameSystem(GameSystemID::RENDERER_SYSTEM, "RENDER_SYSTEM") {}


void RenderSystem::Run() {

}

void RenderSystem::Run(const GameState& state) {

    RenderGameState(state);
}

void RenderSystem::Init() {

    scores.reserve(WaveConstants::NUMBER_OF_ENEMIES);

}

void RenderSystem::AddMessage(const MessageUi &msg) {

    msgs.push(msg);

}

void RenderSystem::AddScore(const MessageUi &score) {

    scores.push_back(score);
}

//--------------------------------------------------------------------------

void RenderSystem::RenderGameState(const GameState& state) {

    std::string text;

    switch (state) {

        case GameState::GAME_OVER:

            RenderGameOver();
            break;

        case GameState::IN_GAME:

            RenderEnemies();
            RenderExplosions();
            RenderShield();
            RenderPlayer();
            RenderProjectiles();
            RenderPowerUps();
            RenderUi();
            break;

        case GameState::BEGIN_WAVE:

            text = "WAVE " + std::to_string(SystemLocator::waveLocator->GetWaveCounter());
            RenderWaveTransition(text);
            break;


        case GameState::END_WAVE:

            text = "WAVE COMPLETED!";
            RenderWaveTransition(text);
            break;
    }
}

void RenderSystem::DrawSprite(const Sprite& sprite, const Vector2& pos, const Vector2& size, const float scaling, const float rotation) const {

    const auto& texture = SystemLocator::assetLocator->GetTexture(sprite.texture);
    const Rectangle dest = {pos.x, pos.y, size.x * scaling, size.y * scaling};
    DrawTexturePro(texture, sprite.src, dest, {0, 0}, rotation, WHITE);
}

/**
 * Renders all enemies and shift their sprites
 * \param world
 */
void RenderSystem::RenderEnemies() const {

    const auto& enemies = SystemLocator::entityLocator->GetEnemies();
    if (enemies.empty()) return;

    const size_t spriteToDraw = static_cast<size_t>(GetTime() * 4) % 2;

    for (const auto& enemy : enemies) {

        if (enemy.wave.state == WaveState::OUT_FORMATION) continue;

        DrawSprite(*enemy.render.sprites[spriteToDraw], enemy.position, enemy.render.size, RenderConstants::ENEMY_SCALING);
        //DrawRectangleLinesEx(MakeDestRect(enemy.position, enemy.render.size, RenderConstants::ENEMY_SCALING), 1.0f, PINK);
    }
}


/**
 * Renders the player
 * \param world
 */
void RenderSystem::RenderPlayer() const {

    const auto* player = SystemLocator::entityLocator->GetPlayer();
    const auto& sprites = SystemLocator::assetLocator->GetPlayerSprite();

    size_t spriteIndex = 0; // middle
    if (player->GetDir() > 0) spriteIndex = 2; // right
    if (player->GetDir() < 0) spriteIndex = 1; // left

    DrawSprite(*sprites[spriteIndex], player->GetPosition(), player->GetSize(), RenderConstants::PLAYER_SCALING);
    //DrawRectangleLinesEx(MakeDestRect(player->GetPosition(), player->GetSize(), RenderConstants::PLAYER_SCALING), 1.0f, PINK);
}

/**
 * Renders the shield
 */
void RenderSystem::RenderShield() const {

    if (!SystemLocator::entityLocator->GetPlayer()->IsShieldActive()) return;

    const auto& shield = SystemLocator::entityLocator->GetShield();
    DrawSprite(*shield.render.sprites[0], shield.position, shield.render.size, RenderConstants::SHIELD_SCALING);
    //DrawRectangleLinesEx(MakeDestRect(shield.position, shield.render.size, RenderConstants::SHIELD_SCALING), 1.0f, PINK);
    RenderShieldUi();
}

/**
 * Renders shield hp as ui element
 */
void RenderSystem::RenderShieldUi() const{

    const auto& shield = SystemLocator::entityLocator->GetShield();
    const auto& sprite = *SystemLocator::assetLocator->GetPowerUpIcon(PowerUpType::SHIELD)[0];

    constexpr Vector2 posStart{20, GameConstants::SCREEN_HEIGHT - 90};
    const int width = sprite.src.width * RenderConstants::SHIELD_UI_SCALING;
    constexpr int spacing = 48;

    for (int i = 0; i < shield.hp; ++i) {

        const Vector2 pos = {posStart.x + (width + spacing * i), posStart.y};
        DrawSprite(sprite, pos, {sprite.src.width, sprite.src.height}, RenderConstants::SHIELD_UI_SCALING);
    }

}

/**
 * Renders all projectiles
 * \param world
 */
void RenderSystem::RenderProjectiles() const {

    const auto& projectiles = SystemLocator::entityLocator->GetProjectiles();
    if (projectiles.empty()) return;

    for (const auto& p : projectiles)
        DrawSprite(*p.render.sprites[0], p.position, p.render.size, RenderConstants::PROJECTILE_SCALING);
        //DrawRectangleLinesEx(MakeDestRect(p.position, p.render.size, RenderConstants::PROJECTILE_SCALING), 1.0f, PINK);
}

void RenderSystem::RenderExplosions() const {

    const auto& explosions = SystemLocator::entityLocator->GetExplosions();
    if (explosions.empty()) return;

    for (const auto& exp : explosions) {

        if (!SystemLocator::timerLocator->IsRunning(exp.lifetime)) continue;
        DrawSprite(*exp.render.sprites[0], exp.position, exp.render.size, RenderConstants::EXPLOSION_SCALING);
        DrawRectangleLinesEx(exp.hitbox,1.0f, PINK);
    }
}


void RenderSystem::RenderPowerUps() const {

    const auto& powerUps = SystemLocator::entityLocator->GetPowerUps();
    if (powerUps.empty()) return;

    for (const auto& p : powerUps)
        DrawSprite(*p.render.sprites[0], p.position, p.render.size, RenderConstants::POWER_UP_SCALING);
        //DrawRectangleLinesEx(MakeDestRect(p.position, p.render.size, RenderConstants::POWER_UP_SCALING), 1.0f, PINK);
}

/**
 * Renders the Game Over screen
 */
void RenderSystem::RenderGameOver() const {

    const auto font = SystemLocator::assetLocator->GetFont();
    constexpr const char* caption    = "GAME OVER";
    constexpr const char* subCaption = "PRESS `ENTER` TO RESTART";

    const Vector2 captionSize    = MeasureTextEx(font, caption, RenderConstants::GAME_OVER_CAPTION_SIZE, RenderConstants::SPACING);
    const Vector2 subCaptionSize = MeasureTextEx(font, subCaption, RenderConstants::GAME_OVER_SUBCAPTION_SIZE, RenderConstants::SPACING);

    const Vector2 captionPos    = {GameConstants::SCREEN_ORIGIN.x - captionSize.x / 2, GameConstants::SCREEN_HEIGHT * 0.2f};
    const Vector2 subCaptionPos = {GameConstants::SCREEN_ORIGIN.x - subCaptionSize.x / 2, captionPos.y + 225};

    DrawTextEx(font, caption,    captionPos,    RenderConstants::GAME_OVER_CAPTION_SIZE,    RenderConstants::SPACING, RED);
    DrawTextEx(font, subCaption, subCaptionPos, RenderConstants::GAME_OVER_SUBCAPTION_SIZE, RenderConstants::SPACING, WHITE);
}

/**
 * Render transition screens between the waves
 * \param caption
 */
void RenderSystem::RenderWaveTransition(const std::string& caption) const {

    const auto font = SystemLocator::assetLocator->GetFont();
    const Vector2 captionSize = MeasureTextEx(font, caption.c_str(), RenderConstants::WAVE_TRANSITION_CAPTION_SIZE, RenderConstants::SPACING);
    constexpr uint32_t xOffset = 30;

    const Vector2 captionPos = {GameConstants::SCREEN_ORIGIN.x - captionSize.x / 2 + xOffset, GameConstants::SCREEN_HEIGHT * 0.22f};
    DrawTextEx(font, caption.c_str(), captionPos, RenderConstants::WAVE_TRANSITION_CAPTION_SIZE, RenderConstants::SPACING, WHITE);
}

void RenderSystem::RenderHighScore() const {

    const auto font = SystemLocator::assetLocator->GetFont();
    constexpr const char* caption = "HIGH SCORE";
    std::string highScore = std::to_string(SystemLocator::scoreLocator->GetHighScore());

    if (highScore.length() < ScoringConstants::HIGHSCORE_DIGITS) {
        highScore.insert(0, ScoringConstants::HIGHSCORE_DIGITS - highScore.length(), '0'); // add leading zeros, if needed
    }

    const Vector2 captionSize   = MeasureTextEx(font, caption, RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING);
    const Vector2 highScoreSize = MeasureTextEx(font, highScore.c_str(), RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING);
    constexpr int xOffset = 30;

    DrawTextEx(font, caption, {GameConstants::SCREEN_ORIGIN.x - captionSize.x / 2 + xOffset, GameConstants::SCREEN_HEIGHT * 0.02f},
        RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING, WHITE);
    DrawTextEx(font, highScore.c_str(), {GameConstants::SCREEN_ORIGIN.x - highScoreSize.x / 2 + xOffset, GameConstants::SCREEN_HEIGHT * 0.055f},
        RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING, WHITE);
}

/**
 * Renders current mult
 */
void RenderSystem::RenderMult() const {

    const auto font = SystemLocator::assetLocator->GetFont();
    const std::string caption = "x" + std::format("{:.2f}", SystemLocator::scoreLocator->GetMult());
    const std::string lvl     = std::to_string(SystemLocator::scoreLocator->GetFlowLvl());
    constexpr Vector2 pos     = {GameConstants::SCREEN_WIDTH - 200, GameConstants::SCREEN_HEIGHT - 100};

    DrawTextEx(font, caption.c_str(), pos,              RenderConstants::MULT_SIZE, RenderConstants::SPACING, WHITE);
    DrawTextEx(font, lvl.c_str(),     {pos.x, pos.y + 50}, RenderConstants::FLOW_LVL_SIZE, RenderConstants::SPACING, WHITE);
}

void RenderSystem::RenderUi() {

    RenderHighScore();
    RenderMult();
    RenderMessages();
    RenderScores();
}

/**
 * Renders the enemy scores/point on enemy death
 */
void RenderSystem::RenderScores() {

    if (scores.empty()) return;

    const auto& font = SystemLocator::assetLocator->GetFont();

    for (auto& score : scores) {

        if (!SystemLocator::timerLocator->IsRunning(score.displayTimer)) continue;
        DrawTextEx(font, score.text.c_str(), {score.pos.x + 12, score.pos.y - 12}, score.fSize, RenderConstants::SPACING, WHITE);
    }
}

/**
 * Render ui messages
 */
void RenderSystem::RenderMessages() {

    if (msgs.empty()) return;

    const auto& font = SystemLocator::assetLocator->GetFont();
    auto& currentMsg = msgs.front();

    const Vector2 msgSize  = MeasureTextEx(font, currentMsg.text.c_str(), RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING);
    const float msgPosX    = GameConstants::SCREEN_ORIGIN.x - msgSize.x / 2 + 40;
    constexpr float msgPosY = GameWorldConstants::playerSpawnY - 100;

    if (!currentMsg.active) {

        SystemLocator::timerLocator->Start(1.0f, currentMsg.displayTimer);
        currentMsg.active = true;
    }

    if (SystemLocator::timerLocator->IsRunning(currentMsg.displayTimer)) {

        DrawTextEx(font, currentMsg.text.c_str(), {msgPosX, msgPosY}, 18, RenderConstants::SPACING, WHITE);
    } else {
        msgs.pop();
    }
}


