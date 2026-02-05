//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include "../GameWorld.h"


class ISystem {

    public:

    virtual ~ISystem() = default;
    virtual void Run(GameWorld& world) = 0;
};


