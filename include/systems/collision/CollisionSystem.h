//
// Created by Michael Hirsch on 21.02.26.
//

#pragma once
#include <raylib.h>

#include "../../game/IGameSystem.h"
#include "entities/Shield.h"


class CollisionSystem : public IGameSystem {


    public:
    CollisionSystem();
    ~CollisionSystem() override = default;

    void Run() override;

    private:

    void CheckPlayerProjectiles();
    void CheckEnemiesProjectiles();
    void CheckPlayerEnemies();
    void CheckPowerUps();
    void CheckExplosions();

    void DamageShield(Shield& shield);

};


