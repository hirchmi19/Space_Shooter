//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Game.h"

#include "GameWorld.h"
#include "../Constants/GameConstants.h"

Game::Game() {

    CreateGame();
}

Game::~Game() {

    world.reset();
    CloseWindow();
};

/**
 * Runs the Game Loop
 */
void Game::Run() const {

    constexpr double timePerUpdate = 1.0 / GameConstants::UPS;

    double prevTime = GetTime();
    double updateAccumulator = 0.0;

    HideCursor();

    while (!WindowShouldClose()) {

        const double currentTime = GetTime();
        updateAccumulator += (currentTime - prevTime) / timePerUpdate;
        prevTime = currentTime;

        while (updateAccumulator >= 1.0) { // ensures an frame independent game loop

            world->RunGameplaySystems();
            updateAccumulator--;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        world->RunRenderSystem();

        EndDrawing();
    }
}

//--------------------------------------------------------------------------


/**
 * Creates a Window and the Game World
 */
void Game::CreateGame() {

    InitWindow(GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT, "Space Shooter");
    SetTargetFPS(GameConstants::TARGET_FPS);
    world = std::make_unique<GameWorld>();
}

