//
// Created by Michael Hirsch on 06.02.26.
//

#pragma once
#include <queue>
#include <string>
#include "../../game/IGameSystem.h"
#include "game/GameState.h"
#include "locators/IRenderLocator.h"
#include "systems/assets/Sprite.h"
#include "systems/rendering/RenderSystem.h"
#include "systems/rendering/MessageUi.h"

enum class GameState;

class RenderSystem : public IRenderLocator, public IGameSystem {

    public:
    RenderSystem();
    ~RenderSystem() override = default;

    void Run() override;
    void Run(const GameState& state);
    void Init() override;

    void AddMessage(const MessageUi& msg) override;
    void AddScore(const MessageUi& score) override;

    void ClearUiElements()  override {
        scores.clear();
        while (!msgs.empty()) msgs.pop();
    }

    private:

    void DrawSprite(const Sprite& sprite, const Vector2& pos, const Vector2& size, float scaling, float rotation = 0.0f) const;

    void RenderPlayer() const;
    void RenderEnemies() const;
    void RenderProjectiles() const;
    void RenderExplosions() const;
    void RenderPowerUps() const;
    void RenderShield() const;
    void RenderShieldUi() const;

    void RenderUi();
    void RenderHighScore() const;
    void RenderMult() const;
    void RenderMessages();
    void RenderScores();

    void RenderGameOver() const;
    void RenderWaveTransition(const std::string& caption) const;
    void RenderGameState(const GameState& state);

    std::queue<MessageUi> msgs;
    std::vector<MessageUi> scores;
};


