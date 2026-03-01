//
// Created by Michael Hirsch on 09.02.26.
//

#pragma once
#include "../IGameSystem.h"
#include "../../Entities/Projectiles/Projectile.h"
#include "../../Entities/Enemies/Enemy.h"

class Player;

class MovementSystem : public IGameSystem{

    public:
    MovementSystem();
    ~MovementSystem() override = default;

    void Run() override;

    private:
    void MovePlayer();
    void MoveProjectiles();
    void MoveEnemies();

    void MoveProjectile(Projectile& projectile);
    void MoveEnemy(Enemy& enemy, const Vector2& newPos);
};


