//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "EntityType.h"
#include "Systems/GameSystem.h"
#include "Systems/ISystem.h"


class GameWorld {

    public:
    GameWorld();
    ~GameWorld();

    private:
    std::unordered_map<EntityType, std::vector<std::unique_ptr<Entity>>> entities;
    GameSystem system;
};


