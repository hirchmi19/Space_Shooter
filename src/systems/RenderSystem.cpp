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

RenderSystem::RenderSystem() : IGameSystem(GameSystemID::RENDERER_SYSTEM, "RENDER_SYSTEM") {}


void RenderSystem::Run() {

}

void RenderSystem::Run(const GameState& state) {

    RenderGameState(state);
}

void RenderSystem::Init() {

    msgs.reserve(WaveConstants::NUMBER_OF_ENEMIES);

}

void RenderSystem::AddMessage(const MessageUi &msg) {

    msgs.push_back(msg);

}

void RenderSystem::AddScore(const MessageUi &score) {

    scores.push_back(score);
}

//--------------------------------------------------------------------------

void RenderSystem::RenderGameState(const GameState& state) {

    std::string text;

    switch (state) {

        case GameState::GAME_OVER:
            RenderGameOver(state);
            break;

        case GameState::IN_GAME:
            RenderShield();
            RenderPlayer();
            RenderEnemies();
            RenderExplosions();
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

/**
 * Renders all enemies and shift their sprites
 * \param world
 */
void RenderSystem::RenderEnemies() const {

    const auto& enemies = SystemLocator::entityLocator->GetEnemies();

    if (enemies.empty()) return;

    const double time = GetTime();
    const size_t spriteToDraw = static_cast<size_t>(time * 4) % 2; // flip between sprites to simulate animation

    for (const auto& enemy : enemies) {

        if (enemy.wave.state == WaveState::OUT_FORMATION) continue;

        const auto& texture = SystemLocator::assetLocator->GetTexture(TextureID::ENEMY_CANVAS);
        const auto& sprite = *(enemy.render.sprites[spriteToDraw]);

        const Rectangle dest = {
            enemy.wave.worldPosition.x,
            enemy.wave.worldPosition.y,
            enemy.render.size.x * RenderConstants::ENEMY_SCALING,
            enemy.render.size.y * RenderConstants::ENEMY_SCALING};

        DrawTexturePro(texture, sprite.src, dest, { 0, 0 }, 0.0f, WHITE);
        //DrawRectangleLinesEx(enemy.combat.hitbox, 1.0f, PINK); // draw hitbox
    }
}


/**
 * Renders the player
 * \param world
 */
void RenderSystem::RenderPlayer() const {

    const auto player = SystemLocator::entityLocator->GetPlayer();
    const auto& playerTexture = SystemLocator::assetLocator->GetTexture(TextureID::PLAYER_SHIP_CANVAS);
    auto playerSprite = SystemLocator::assetLocator->GetSprite(SpriteID::PLAYER_SHIP_MIDDLE);

    const Rectangle dest = {
        player->GetPosition().x,
        player->GetPosition().y,
        player->GetSize().x * RenderConstants::PLAYER_SCALING,
        player->GetSize().y * RenderConstants::PLAYER_SCALING
    };

    const auto pSpeed = player->GetDir();
    if (pSpeed > 0) playerSprite = SystemLocator::assetLocator->GetSprite(SpriteID::PLAYER_SHIP_RIGHT);
    if (pSpeed < 0) playerSprite = SystemLocator::assetLocator->GetSprite(SpriteID::PLAYER_SHIP_LEFT);

    DrawTexturePro(playerTexture, playerSprite.src, dest, Vector2{0, 0}, 0.0f, WHITE);
    //DrawRectangleLinesEx(player->GetHitBox(), 1.0f, PINK); // draw hitbox
}

void RenderSystem::RenderShield() const {

    if (!SystemLocator::entityLocator->GetPlayer()->IsShieldActive()) return;

    const auto& shield = SystemLocator::entityLocator->GetShield();
    const auto& texture = SystemLocator::assetLocator->GetTexture(TextureID::EFFECT_CANVAS);
    const auto sprite = shield.render.sprites[0];

    const Rectangle dest = {
        shield.position.x,
        shield.position.y,
        shield.render.size.x * RenderConstants::SHIELD_SCALING,
        shield.render.size.y * RenderConstants::SHIELD_SCALING
    };

    DrawTexturePro(texture, sprite->src, dest, Vector2{0, 0}, 0.0f, WHITE);
    RenderShieldUi();
    //DrawRectangleLinesEx(shield.hitbox, 1.0f, PINK); // draw hitbox
}

void RenderSystem::RenderShieldUi() const{

    const auto& shield = SystemLocator::entityLocator->GetShield();
    const auto& texture = SystemLocator::assetLocator->GetTexture(TextureID::EFFECT_CANVAS);
    const auto& sprite = &SystemLocator::assetLocator->GetSprite(SpriteID::SHIELD_UI);

    constexpr Vector2 posStart{70, GameConstants::SCREEN_HEIGHT - 100};
    const int width = sprite->src.x * RenderConstants::SHIELD_UI_SCALING;
    constexpr int spacing = 48;

    for (int i = 0; i < shield.hp; ++i) {

        const Vector2 pos = {posStart.x + (width + spacing  * i), posStart.y};

        const Rectangle dest = {

            pos.x , pos.y,
            sprite->src.width * RenderConstants::SHIELD_UI_SCALING,
            sprite->src.height * RenderConstants::SHIELD_UI_SCALING

        };

        DrawTexturePro(texture,sprite->src, dest, Vector2{0, 0}, 0.0f, WHITE);

    }

}

/**
 * Renders all projectiles
 * \param world
 */
void RenderSystem::RenderProjectiles() const  {

    const auto& projectiles = SystemLocator::entityLocator->GetProjectiles();

    if (projectiles.empty()) return;

    for (const auto& projectile : projectiles ) {

        const TextureID& texId = projectile.render.sprites[0]->texture;
        const auto& texture = SystemLocator::assetLocator->GetTexture(texId);
        const auto& sprite = *(projectile.render.sprites[0]);

        const Rectangle dest = {
            projectile.movement.position.x,
            projectile.movement.position.y,
            projectile.render.size.x * RenderConstants::PROJECTILE_SCALING,
            projectile.render.size.y * RenderConstants::PROJECTILE_SCALING};

        DrawTexturePro(texture, sprite.src, dest, { 0, 0 }, 0.0f, WHITE);
       // DrawRectangleLinesEx(projectile.combat.hitbox, 1.0f, PINK); // draw hitbox
    }
}

void RenderSystem::RenderExplosions() const {


    const auto& explosions = SystemLocator::entityLocator->GetExplosions();

    if (explosions.empty()) return;

    for (const auto& exp : explosions ) {

        const TextureID& texId = exp.render.sprites[0]->texture;
        const auto& texture = SystemLocator::assetLocator->GetTexture(texId);
        const auto& sprite = *exp.render.sprites[0];

        const Rectangle dest = {
            exp.center.x,
            exp.center.y,
            exp.render.size.x * RenderConstants::EXPLOSION_SCALING ,
            exp.render.size.y * RenderConstants::EXPLOSION_SCALING};


       if (!SystemLocator::timerLocator->IsRunning(exp.timer)) continue;
        DrawTexturePro(texture, sprite.src, dest, { 0, 0 }, 0.0f, WHITE);
    }
}

void RenderSystem::RenderPowerUps() const {

    const auto& powerUps = SystemLocator::entityLocator->GetPowerUps();

    if (powerUps.empty()) return;

    for (const auto& powerUp : powerUps ) {

        const TextureID& texId = powerUp.render.sprites[0]->texture;
        const auto& texture = SystemLocator::assetLocator->GetTexture(texId);
        const auto& sprite = *powerUp.render.sprites[0];

        const Rectangle dest = {
            powerUp.movement.position.x,
            powerUp.movement.position.y,
            powerUp.render.size.x * RenderConstants::POWER_UP_SCALING,
            powerUp.render.size.y * RenderConstants::POWER_UP_SCALING};

        DrawTexturePro(texture, sprite.src, dest, { 0, 0 }, 0.0f, WHITE);
        //DrawRectangleLinesEx(powerUp.hitbox, 1.0f, PINK); // draw hitbox

    }
}

void RenderSystem::RenderGameOver(const GameState& state) const {

    if (state != GameState::GAME_OVER) return;

    const auto font = SystemLocator::assetLocator->GetFont();
    constexpr std::string caption = "GAME OVER";
    const std::string subCaption = "PRESS `ENTER` TO RESTART";

    const Vector2 captionWidth = MeasureTextEx(font, caption.c_str(), RenderConstants::GAME_OVER_CAPTION_SIZE, RenderConstants::SPACING);
    const Vector2 subCaptionWidth = MeasureTextEx(font, subCaption.c_str(), RenderConstants::GAME_OVER_SUBCAPTION_SIZE, RenderConstants::SPACING);

    const Vector2 captionPos = {GameConstants::SCREEN_ORIGIN.x - captionWidth.x / 2, GameConstants::SCREEN_HEIGHT * 0.2f};
    const Vector2 subCaptionPos = {GameConstants::SCREEN_ORIGIN.x - subCaptionWidth.x / 2, captionPos.y + 225};

    DrawTextEx(font, caption.c_str(), captionPos, RenderConstants::GAME_OVER_CAPTION_SIZE, RenderConstants::SPACING, RED);
    DrawTextEx(font, subCaption.c_str(), subCaptionPos, RenderConstants::GAME_OVER_SUBCAPTION_SIZE, RenderConstants::SPACING, WHITE);
}

void RenderSystem::RenderWaveTransition(const std::string& caption) const {

    const auto font = SystemLocator::assetLocator->GetFont();
    const Vector2 captionWidth = MeasureTextEx(font, caption.c_str(), RenderConstants::WAVE_TRANSITION_CAPTION_SIZE, RenderConstants::SPACING);
    constexpr uint32_t xOffset = 30;

    const Vector2 captionPos = {GameConstants::SCREEN_ORIGIN.x - captionWidth.x / 2 + xOffset, GameConstants::SCREEN_HEIGHT * 0.22f};
    DrawTextEx(font, caption.c_str(), captionPos, RenderConstants::WAVE_TRANSITION_CAPTION_SIZE, RenderConstants::SPACING, WHITE);
}


void RenderSystem::RenderUi() {

    RenderHighScore();
    RenderMult();
    RenderScores();
    RenderMessages();
}

void RenderSystem::RenderHighScore() const {

    const auto font = SystemLocator::assetLocator->GetFont();
    constexpr std::string caption = "HIGH SCORE";
    std::string highScore = std::to_string(SystemLocator::scoreLocator->GetHighScore());

    if (highScore.length() < ScoringConstants::HIGHSCORE_DIGITS) { // add leading zeros, if needed

        const uint32_t delta = ScoringConstants::HIGHSCORE_DIGITS - highScore.length();
        highScore.insert(0, delta, '0');

    }

    const Vector2 captionWidth = MeasureTextEx(font, caption.c_str(), RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING);
    const Vector2 highScoreWidth = MeasureTextEx(font, highScore.c_str(), RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING);
    constexpr int xOffset = 30;

    const Vector2 captionPos = {GameConstants::SCREEN_ORIGIN.x - captionWidth.x / 2 + xOffset, GameConstants::SCREEN_HEIGHT * 0.02f};
    const Vector2 highScorePos = {GameConstants::SCREEN_ORIGIN.x - highScoreWidth.x / 2 + xOffset, GameConstants::SCREEN_HEIGHT * 0.055f};

    DrawTextEx(font, caption.c_str(), captionPos, RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING, WHITE);
    DrawTextEx(font, highScore.c_str(), highScorePos, RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING, WHITE);

}

void RenderSystem::RenderMult() const {

    const auto font = SystemLocator::assetLocator->GetFont();
    const auto& mult = SystemLocator::scoreLocator->GetMult();
    const auto& flowLvl = SystemLocator::scoreLocator->GetFlowLvl();

    std::string caption = "x" + std::format("{:.2f}", mult);
    std::string lvl = std::to_string(flowLvl);
    const Vector2 pos = {GameConstants::SCREEN_WIDTH - 200, GameConstants::SCREEN_HEIGHT - 100};

    DrawTextEx(font, caption.c_str(), pos, 25, RenderConstants::SPACING, WHITE);
    DrawTextEx(font, lvl.c_str(), {pos.x, pos.y + 50}, 20, RenderConstants::SPACING, WHITE);

}

void RenderSystem::RenderScores() {

    if (scores.empty()) return;

    const auto& font = SystemLocator::assetLocator->GetFont();

    for (auto& score : scores) {

        if (!SystemLocator::timerLocator->IsRunning(score.displayTimer)) continue;

        const Vector2 scoresPos = {score.pos.x + 12, score.pos.y - 12};
        DrawTextEx(font, score.text.c_str(), scoresPos, score.fSize, RenderConstants::SPACING, WHITE);

    }
}

void RenderSystem::RenderMessages() {

    if (msgs.empty()) return;

    const auto& font = SystemLocator::assetLocator->GetFont();
    auto& currentMsg = msgs.front();

    const Vector2 msgWidth = MeasureTextEx(font, currentMsg.text.c_str(), RenderConstants::HIGHSCORE_CAPTION_SIZE, RenderConstants::SPACING);
    const float msgPosX = GameConstants::SCREEN_ORIGIN.x - msgWidth.x / 2 + 40;
    constexpr float msgPosY = GameWorldConstants::playerSpawnY - 100;

    if (!currentMsg.active) {

            SystemLocator::timerLocator->Start(0.6f, currentMsg.displayTimer);
            currentMsg.active = true;
    }
    if (SystemLocator::timerLocator->IsRunning(currentMsg.displayTimer) && currentMsg.active) {

        DrawTextEx(font, currentMsg.text.c_str(), {msgPosX, msgPosY}, 15, RenderConstants::SPACING, WHITE);

    } else msgs.erase(msgs.begin());

}

