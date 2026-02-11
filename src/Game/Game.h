//
// Created by Michael Hirsch on 03.02.26.
//

#pragma once
#include <memory>

class GameWorld;

class Game {

    public:
    Game();
    ~Game();
    void Run();

    private:
    std::unique_ptr<GameWorld> world;

    void CreateGame();
};
