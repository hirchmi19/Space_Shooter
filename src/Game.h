//
// Created by Michael Hirsch on 03.02.26.
//

#pragma once

class Game {

    public:
    Game();
    ~Game();

    void Run();

    private:
    static void Init();
    void Update();
    void Draw();
};