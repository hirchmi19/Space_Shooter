//
// Created by Michael Hirsch on 06.02.26.
//

#pragma once

#include <string>

#include "../../game/IGameSystem.h"
#include "game/GameState.h"
#include "locators/IRenderLocator.h"
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

    void ClearUi()  override {msgs.clear(); scores.clear();};

    private:

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

    void RenderGameOver(const GameState& state) const;
    void RenderWaveTransition(const std::string& caption) const;
    void RenderGameState(const GameState& state);

    std::vector<MessageUi> msgs;
    std::vector<MessageUi> scores;
};


