//
// Created by Michael Hirsch on 21.02.26.
//

#pragma once
#include "../../game/IGameSystem.h"


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

};


