//
// Created by Michael Hirsch on 04.02.26.
//

#include <iostream>
#include <ostream>
#include "game/GameWorld.h"
#include "game/GameState.h"
#include "constants/GameWorldConstants.h"
#include "systems/systems.h"

GameWorld::GameWorld() {

    CreateSystems();
    InitGameSystems();

}

/**
 * Runs all gameplay systems by calling their run/update functions
 */
void GameWorld::RunGameplaySystems() {

    const auto& bgSys = gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)];
    const auto& entSys = gameSystems[ToIndex(GameSystemID::ENTITY_SYSTEM)];
    const auto& waveSys = gameSystems[ToIndex(GameSystemID::WAVE_SYSTEM)];
    const auto& moveSys = gameSystems[ToIndex(GameSystemID::MOVEMENT_SYSTEM)];
    const auto& collSys = gameSystems[ToIndex(GameSystemID::COLLISION_SYSTEM)];
    const auto& timerSys = gameSystems[ToIndex(GameSystemID::TIMER_SYSTEM)];

    if (bgSys) bgSys->Run();   // background is always moving
    if (timerSys) timerSys->Run(); // updates timers

    switch (currentGameState) {

        case GameState::BEGIN_WAVE:

            if (!timerStarted) {

                SystemLocator::timerLocator->Start(3.0f, transitionTimer);
                timerStarted = true;
            }

            if (!SystemLocator::timerLocator->IsRunning(transitionTimer) && timerStarted) {

                currentGameState = GameState::IN_GAME;
                timerStarted = false;
                if (waveSys) GetGameSystemStatic<WaveSystem>(GameSystemID::WAVE_SYSTEM).Start();
            }
            break;

        case GameState::IN_GAME:

            if (entSys)
                GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).HandleInputs(); // 1. read inputs (begin of frame)

            if (waveSys) waveSys->Run(); // 2. let the gameplay systems do their thing
            if (moveSys) moveSys->Run();
            if (collSys) collSys->Run();

            if (entSys && !GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).PlayerAlive())
                currentGameState = GameState::GAME_OVER; // 3. check if game is over

            if (entSys) entSys->Run(); // 4. remove dead entities

            if (GetGameSystemStatic<WaveSystem>(GameSystemID::WAVE_SYSTEM).waveFinished) currentGameState = GameState::END_WAVE; // 5. check if wave is over

            GetGameSystemStatic<TimerSystem>(GameSystemID::TIMER_SYSTEM).KillTimers(); // 6. remove disposable timers

            break;

        case GameState::END_WAVE:

            GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).ClearEntities();

            if (!timerStarted) {

                SystemLocator::timerLocator->Start(3.f, transitionTimer);
                timerStarted = true;
            }

            if (!SystemLocator::timerLocator->IsRunning(transitionTimer) && timerStarted) {

                currentGameState = GameState::BEGIN_WAVE;
                timerStarted = false;
            }

            break;

        case GameState::GAME_OVER:

            GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).ClearEntities();
            if (IsKeyPressed(KEY_ENTER)) Restart();
            break;
    }

}

/**
 * Renders EVERYTHING
 */
void GameWorld::RunRenderSystem() {

    if (gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)])
        GetGameSystemStatic<BackgroundSystem>(GameSystemID::BACKGROUND_SYSTEM).Render(); // render bg
    if (gameSystems[ToIndex(GameSystemID::RENDERER_SYSTEM)])
        GetGameSystemStatic<RenderSystem>(GameSystemID::RENDERER_SYSTEM).Run(currentGameState); // render current game state

}

//--------------------------------------------------------------------------

/**
 * Constructs all game systems
 */
void GameWorld::CreateSystems() {

    const auto assetSys = std::make_shared<AssetSystem>();
    SystemLocator::assetLocator = assetSys;
    AddSystem(assetSys);

    const auto entSys = std::make_shared<EntitySystem>();
    SystemLocator::entityLocator = entSys;
    AddSystem(entSys);

    AddSystem(std::make_shared<BackgroundSystem>());
    AddSystem(std::make_shared<RenderSystem>());
    AddSystem(std::make_shared<MovementSystem>());
    AddSystem(std::make_shared<CollisionSystem>());

    const auto waveSys = std::make_shared<WaveSystem>();
    SystemLocator::waveLocator = waveSys;
    AddSystem(waveSys);

    const auto scoreSys = std::make_shared<ScoreSystem>();
    SystemLocator::scoreLocator = scoreSys;
    AddSystem(scoreSys);

    const auto timerSys = std::make_shared<TimerSystem>();
    SystemLocator::timerLocator = timerSys;
    AddSystem(timerSys);
}

/**
 * Adds a system and gives GameWorld the ownership
 * \param system
 */
void GameWorld::AddSystem(std::shared_ptr<IGameSystem> system) {

    const std::string name(system->GetSystemName());
    const GameSystemID id = system->GetSystemID();
    const size_t index = ToIndex(id);
    gameSystems[index] = std::move(system);

    std::cout << "ADDED SYSTEM:  " << name << std::endl;
}

/**
 * Inits all game systems
 */
void GameWorld::InitGameSystems() {

    transitionTimer = SystemLocator::timerLocator->CreateTimer(0.0f, false);

    for (const auto& gameSystem : gameSystems) {

        if (!gameSystem) {
            std::cout << "GAME SYSTEM IS NULL, skipping..." << std::endl;
            continue;
        }

        std::string name(gameSystem->GetSystemName());
        gameSystem->Init();
        std::cout << "GAME SYSTEM INITIALIZED: " << name << std::endl;
    }
}

/**
 * Resets the playing state machine
 */
void GameWorld::Restart() {

    const auto player = GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).GetPlayer();

    GetGameSystemStatic<WaveSystem>(GameSystemID::WAVE_SYSTEM).ResetWaveCounter();
    GetGameSystemStatic<ScoreSystem>(GameSystemID::SCORE_SYSTEM).ResetScore();
    player->Revive();
    player->SetPosition(GameWorldConstants::playerSpawn);

    currentGameState = GameState::BEGIN_WAVE;
}



