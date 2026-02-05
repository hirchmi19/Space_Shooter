//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include <vector>

#include "ISystem.h"


class GameSystem {

    public:
    GameSystem();
    ~GameSystem();

    void AddSystem(std::unique_ptr<ISystem>);
    void RunSystem(GameWorld& world);

    private:
    std::vector<std::unique_ptr<ISystem>> systems;
};
