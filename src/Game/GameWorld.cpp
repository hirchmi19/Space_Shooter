//
// Created by Michael Hirsch on 04.02.26.
//

#include "GameWorld.h"
#include <cassert>
#include <iostream>
#include <ostream>

#include "../Constants/GameWorldConstants.h"
#include "../Systems/Assets/AssetSystem.h"
#include "../Systems/Collision/CollisionSystem.h"
#include "../Systems/Movement/MovementSystem.h"
#include  "../Systems/Rendering/RenderSystem.h"
#include "../Systems/Scoring/ScoreSystem.h"
#include "../Systems/Waving/WaveSystem.h"
#include "Systems/Entities/EntitySystem.h"
#include "Systems/Rendering/BackgroundSystem.h"
#include "Systems/Timer/TimerSystem.h"

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

    transitionTimer.Tick(1 / GameConstants::UPS);

    switch (currentGameState) {

        case GameState::BEGIN_WAVE:

            if (!timerStarted) {

                transitionTimer.Start(3.f);
                timerStarted = true;
            }

            if (transitionTimer.IsFinished() && timerStarted) {

                currentGameState = GameState::IN_GAME;
                timerStarted = false;
                if (waveSys) GetGameSystemStatic<WaveSystem>(GameSystemID::WAVE_SYSTEM).Start();
            }
            break;

        case GameState::IN_GAME:

            if (entSys)
                GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).HandleInputs(); // 1. read inputs (begin of frame)

            if (timerSys) timerSys->Run(); // 2. updates timers

            if (waveSys) waveSys->Run(); // 3. let the gameplay systems do their thing
            if (moveSys) moveSys->Run();
            if (collSys) collSys->Run();

            if (entSys && !GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).PlayerAlive())
                currentGameState = GameState::GAME_OVER; // 4. check if game is over

            if (entSys) entSys->Run(); // 5. remove dead entities

            if (GetGameSystemStatic<WaveSystem>(GameSystemID::WAVE_SYSTEM).waveFinished) currentGameState = GameState::END_WAVE; // 6. check if wave is over

            GetGameSystemStatic<TimerSystem>(GameSystemID::TIMER_SYSTEM).KillTimers(); // 7. remove disposable timers

            break;

        case GameState::END_WAVE:

            GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).ClearEntities();

            if (!timerStarted) {

                transitionTimer.Start(3.f);
                timerStarted = true;
            }

            if (transitionTimer.IsFinished() && timerStarted) {

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
void GameWorld::InitGameSystems() const {

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



