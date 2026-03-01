//
// Created by Michael Hirsch on 06.02.26.
//

#pragma once
#include <string>
#include "../IGameSystem.h"
#include "../../Game/GameState.h"

class RenderSystem : public IGameSystem {

    public:
    RenderSystem();
    ~RenderSystem() override = default;

    void Run() override;
    void Run(const GameState& state) const;

    private:

    void RenderPlayer() const;
    void RenderEnemies() const;
    void RenderProjectiles() const;
    void RenderHighScore() const;

    void RenderGameOver(const GameState& state) const;
    void RenderWaveTransition(const std::string& caption) const;
    void RenderGameState(const GameState& state) const;

};


