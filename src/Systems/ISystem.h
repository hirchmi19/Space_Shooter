//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once

class GameWorld;


class ISystem {

    public:

    virtual ~ISystem() = default;
    virtual void Run(GameWorld& world) = 0;
    virtual void InitSprites() = 0;
};


