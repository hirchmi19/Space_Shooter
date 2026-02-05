//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Game.h"
#include "Constants/GameConstants.h"

Game::Game() {

    Init();
    world = GameWorld();
    gameSystem =
}

Game::~Game() {

    CloseWindow();
};

void Game::Init() {

    InitWindow(GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT, "Space Shooter");
    SetTargetFPS(GameConstants::TARGET_FPS);
}

void Game::Run() {

    constexpr double timePerUpdate = 1.0 / GameConstants::UPS;

    double prevTime = GetTime();
    double updateAccumulator = 0.0;

    while (!WindowShouldClose()) {

        const double currentTime = GetTime();
        updateAccumulator += (currentTime - prevTime) / timePerUpdate;
        prevTime = currentTime;

        while (updateAccumulator >= 1.0) {

            Update();
            updateAccumulator--;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        Draw();

        EndDrawing();
    }
}

void Game::Draw() {

}

void Game::Update() {

}
