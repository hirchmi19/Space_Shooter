//
// Created by Michael Hirsch on 03.02.26.
//

#pragma once
#include "GameWorld.h"
#include "Systems/GameSystem.h"

class Game {

    public:
    Game();
    ~Game();

    void Run();

    private:

    GameWorld world;

    static void Init();
    void Update();
    void Draw();
};
