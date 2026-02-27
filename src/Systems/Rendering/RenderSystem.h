//
// Created by Michael Hirsch on 06.02.26.
//

#pragma once
#include <string>

#include "raylib.h"
#include "../IGameSystem.h"
#include "../../Entities/Components/TimerComponent.h"

enum class GameState : uint32_t;

class RenderSystem : public IGameSystem {

    public:
    RenderSystem();
    ~RenderSystem() override = default;

    void Run(GameWorld& world) override;

    private:

    void RenderPlayer(const GameWorld& world) const;
    void RenderEnemies(GameWorld& world) const;
    void RenderProjectiles(GameWorld& world) const;
    void RenderHighScore(const GameWorld& world) const;

    void RenderGameOver(const GameWorld& world) const;
    void RenderWaveTransition(const GameWorld& world, const std::string& caption) const;
    void RenderGameState(GameWorld& world, GameState state) const;

};


