//
// Created by Michael Hirsch on 06.02.26.
//

#pragma once
#include "../IGameSystem.h"

class RenderSystem : public IGameSystem {

    public:
    RenderSystem();
    ~RenderSystem() override = default;

    void Run(GameWorld& world) override;

    private:
    void RenderPlayer(const GameWorld& world) const;
    void RenderEnemies(const GameWorld& world) const;
    void RenderBackground(const GameWorld& world) const;
    void RenderProjectiles(GameWorld& world) const;

};


