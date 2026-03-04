//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once

#include "GameSystemID.h"

class IGameSystem {

    public:

    IGameSystem(const GameSystemID id, const char* name) : id(id), systemName(name) {};
    virtual ~IGameSystem() = default;

    virtual void Run() = 0;
    virtual void Init(){};
    virtual void InitTimers(){};

    GameSystemID GetSystemID() const { return id; }
    const char* GetSystemName() const { return systemName; }

    private:
    GameSystemID id;
    const char* systemName;
};


