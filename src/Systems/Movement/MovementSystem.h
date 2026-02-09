//
// Created by Michael Hirsch on 09.02.26.
//

#pragma once
#include "../IGameSystem.h"

//Movement system constants
constexpr uint32_t PLAYER_WORLD_SPEED = 4;
//----------------------------------------------------------------------

class MovementSystem : public IGameSystem{

    public:
    MovementSystem();
    ~MovementSystem() override = default;

    void Run(GameWorld& world) override;

    private:
    void MovePlayer(GameWorld& world);
    void MoveProjectiles(GameWorld& world);
    void MoveEnemies(GameWorld& world);
};


