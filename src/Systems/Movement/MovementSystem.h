//
// Created by Michael Hirsch on 09.02.26.
//

#pragma once
#include "../IGameSystem.h"
#include "../../Entities/Projectile.h"
#include "../../Entities/Enemies/Enemy.h"

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


    void MovePlayer(Player& player);
    void MoveProjectile(Projectile& projectile);
    void MoveEnemy(Enemy& enemy, const Vector2& newPos);
};


