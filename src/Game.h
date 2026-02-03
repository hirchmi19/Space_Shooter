//
// Created by Michael Hirsch on 03.02.26.
//

#pragma once

class Game {

    public:
    Game();
    ~Game();

    private:
    static void InitGameLoop();
    void UpdateGameLoop();
    void DrawGameLoop();
};