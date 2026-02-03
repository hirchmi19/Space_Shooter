//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Game.h"

Game::Game() {

    InitWindow(900, 600, "Space Shooter");
    SetTargetFPS(60);

    Init();
}

Game::~Game() {

    CloseWindow();
};

void Game::Init() {


    InitWindow(900, 600, "Space Shooter");
    SetTargetFPS(60);
}

void Game::Run() {

    constexpr double UPS = 60.0;
    constexpr double timePerUpdate = 1.0 / UPS;

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

        Draw();
    }
}

void Game::Draw() {

}

void Game::Update() {

}
