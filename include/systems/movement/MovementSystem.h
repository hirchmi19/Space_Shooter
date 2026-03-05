//
// Created by Michael Hirsch on 09.02.26.
//

#pragma once
#include "../../game/IGameSystem.h"
#include "../../entities/projectiles/Projectile.h"
#include "../../entities/enemies/Enemy.h"

class Player;

class MovementSystem : public IGameSystem{

    public:
    MovementSystem();
    ~MovementSystem() override = default;

    void Run() override;

    private:

    void MovePlayer();
    void MoveProjectiles();
    void MovePowerUps();
    void MoveEnemies();

    void MoveProjectile(Projectile& projectile);
    void MoveEnemy(Enemy& enemy, const Vector2& newPos);
};


