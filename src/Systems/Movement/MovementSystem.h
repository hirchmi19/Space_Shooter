//
// Created by Michael Hirsch on 09.02.26.
//

#pragma once
#include "../IGameSystem.h"
#include "../../Entities/Projectile.h"

class Player;

class MovementSystem : public IGameSystem{

    public:
    MovementSystem();
    ~MovementSystem() override = default;

    void Run(GameWorld& world) override;

    private:
    void MovePlayer(GameWorld& world);
    void MoveProjectiles(GameWorld& world);
    void MoveEnemies(GameWorld& world);


    void CalcNewPlayerPosition(int32_t speed, Player& player);
    void CalcNewProjectilePosition(Projectile& projectile);
};


