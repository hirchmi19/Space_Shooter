//
// Created by Michael Hirsch on 04.02.26.
//

#include <iostream>
#include <ostream>
#include "game/GameWorld.h"
#include "game/GameState.h"
#include "constants/GameWorldConstants.h"
#include "constants/TimerDurations.h"

GameWorld::GameWorld() {

    CreateSystems();
    InitGameSystems();

}

/**
 * Runs all gameplay systems by calling their run/update functions
 */
void GameWorld::RunGameplaySystems() {

    auto& bgSys    = GetGameSystemStatic<BackgroundSystem>(GameSystemID::BACKGROUND_SYSTEM);
    auto& entSys   = GetGameSystemStatic<EntitySystem>    (GameSystemID::ENTITY_SYSTEM);
    auto& waveSys  = GetGameSystemStatic<WaveSystem>      (GameSystemID::WAVE_SYSTEM);
    auto& moveSys  = GetGameSystemStatic<MovementSystem>  (GameSystemID::MOVEMENT_SYSTEM);
    auto& collSys  = GetGameSystemStatic<CollisionSystem> (GameSystemID::COLLISION_SYSTEM);
    auto& timerSys = GetGameSystemStatic<TimerSystem>     (GameSystemID::TIMER_SYSTEM);
    auto& scoreSys = GetGameSystemStatic<ScoreSystem>     (GameSystemID::SCORE_SYSTEM);


     bgSys.Run();   // background is always moving
     timerSys.Run(); // updates timers

    switch (currentGameState) {

        case GameState::BEGIN_WAVE:

            if (!timerStarted) {

                SystemLocator::timerLocator->Start(TimerDurations::TRANSITION_GW, transitionTimer);
                timerStarted = true;
            }

            if (!SystemLocator::timerLocator->IsRunning(transitionTimer) && timerStarted) {

                currentGameState = GameState::IN_GAME;
                timerStarted = false;
                waveSys.Start();
            }
            break;

        case GameState::IN_GAME:

            entSys.HandleInputs(); // 1. read inputs (begin of frame)

            waveSys.Run(); // 2. let the gameplay systems do their thing
            moveSys.Run();
            collSys.Run();
            scoreSys.Run();

            if (!entSys.PlayerAlive()) {

                entSys.ClearEntities();
                currentGameState = GameState::GAME_OVER; // 3. check if game is over
                break;
            }

            entSys.Run(); // 4. remove dead entities

            if (waveSys.waveFinished) { // 5. check if wave is over

                currentGameState = GameState::END_WAVE;
            }

            break;

        case GameState::END_WAVE:

            if (!timerStarted) {

                SystemLocator::timerLocator->Start(TimerDurations::TRANSITION_GW, transitionTimer);
                timerStarted = true;

                SystemLocator::renderLocator->ClearUiElements(); // clean up and prepeare next wave
                SystemLocator::timerLocator->KillTimers();
                scoreSys.ResetMult();
                entSys.ClearEntities();
               // entSys.GetPlayer()->LeaveFlowState();
            }

            if (!SystemLocator::timerLocator->IsRunning(transitionTimer) && timerStarted) {

                currentGameState = GameState::BEGIN_WAVE;
                timerStarted = false;
            }

            break;

        case GameState::GAME_OVER:

            if (IsKeyPressed(KEY_ENTER)) Restart();
            break;
    }

}

/**
 * Renders EVERYTHING
 */
void GameWorld::RunRenderSystem() {

    if (gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)])
        GetGameSystemStatic<BackgroundSystem>(GameSystemID::BACKGROUND_SYSTEM).Render(); // render starfield background
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
    AddSystem(std::make_shared<MovementSystem>());
    AddSystem(std::make_shared<CollisionSystem>());

    const auto renderSys = std::make_shared<RenderSystem>();
    SystemLocator::renderLocator = renderSys;
    AddSystem(renderSys);

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

    SystemLocator::renderLocator->ClearUiElements();
    SystemLocator::timerLocator->KillTimers();
    SystemLocator::entityLocator->GetPlayer()->LeaveFlowState();
    SystemLocator::entityLocator->Reset();

    player->Revive();
    player->SetPosition(GameWorldConstants::playerSpawn);

    currentGameState = GameState::BEGIN_WAVE;
}



