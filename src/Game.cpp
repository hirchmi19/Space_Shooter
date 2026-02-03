//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Game.h"

Game::Game() {

    InitWindow(900, 600, "Space Shooter");
    SetTargetFPS(60);

    InitGameLoop();

    CloseWindow();
}

Game::~Game() = default;

void Game::InitGameLoop() {

    constexpr double UPS = 60;
    constexpr double timePerUpdate = 1 / UPS;

    double prevTime = GetTime();
    double deltaTime = 0;

    while (!WindowShouldClose()) {

        double currentTime = GetTime();

        deltaTime += (currentTime - prevTime) / timePerUpdate;
        prevTime = currentTime;

        while (deltaTime >= 1) {

           // UpdateGameLoop();
            deltaTime--;
        }

        BeginDrawing();

       // DrawGameLoop();
        ClearBackground(BLACK);

        DrawText("It works!", 300, 280, 20, RAYWHITE);

        EndDrawing();
    }
}

void Game::DrawGameLoop() {

}

void Game::UpdateGameLoop() {

}
