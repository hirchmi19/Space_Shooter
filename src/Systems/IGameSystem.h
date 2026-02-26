//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include "GameSystemID.h"

class GameWorld;


class IGameSystem {

    public:

    IGameSystem(const GameSystemID id) : id(id) {};
    virtual ~IGameSystem() = default;
    virtual void Run(GameWorld& world) = 0;
    const GameSystemID GetSystemID() const { return id; }

    private:
    GameSystemID id;
};


