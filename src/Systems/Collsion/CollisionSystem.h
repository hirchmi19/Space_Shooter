//
// Created by Michael Hirsch on 21.02.26.
//

#pragma once
#include "../IGameSystem.h"


class CollisionSystem : public IGameSystem {


    public:
    CollisionSystem();
    ~CollisionSystem() override = default;

    void Run(GameWorld &world) override;

    private:
    void CheckPlayerProjectiles(GameWorld& world);
    void CheckEnemiesProjectiles(GameWorld& world);
    void CheckPlayerEnemies(GameWorld& world);

};


