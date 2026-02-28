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

    if (bgSys) bgSys->Run(*this);   // background is always moving

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

            if (waveSys) waveSys->Run(*this); // 2. let the gameplay systems do their thing
            if (moveSys) moveSys->Run(*this);
            if (collSys) collSys->Run(*this);

            if (entSys && !GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).PlayerAlive())
                currentGameState = GameState::GAME_OVER; // 3. check if game is over

           if (entSys) entSys->Run(*this); // 4. entities spawn and destruction

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

    const auto renderSys = gameSystems[ToIndex(GameSystemID::RENDERER_SYSTEM)];

    GetGameSystemStatic<BackgroundSystem>(GameSystemID::BACKGROUND_SYSTEM).Render();
    if (renderSys) renderSys->Run(*this);

}

//--------------------------------------------------------------------------

/**
 * Inits all game systems
 */
void GameWorld::CreateSystems() {

    const auto assetSystem = std::make_shared<AssetSystem>();
    SystemLocator::assetLocator = assetSystem;
    AddSystem(assetSystem);

    const auto entSys = std::make_shared<EntitySystem>();
    SystemLocator::entityLocator = entSys;
    AddSystem(entSys);

    AddSystem(std::make_shared<BackgroundSystem>());
    AddSystem(std::make_shared<RenderSystem>());
    AddSystem(std::make_shared<MovementSystem>());
    AddSystem(std::make_shared<WaveSystem>());
    AddSystem(std::make_shared<CollisionSystem>());

    const auto scoreSystem = std::make_shared<ScoreSystem>();
    SystemLocator::scoreLocator = scoreSystem;
    AddSystem(scoreSystem);
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

void GameWorld::Restart() {

    const auto player = GetGameSystemStatic<EntitySystem>(GameSystemID::ENTITY_SYSTEM).getPlayer();

    GetGameSystemStatic<WaveSystem>(GameSystemID::WAVE_SYSTEM).ResetWaveCounter();
    GetGameSystemStatic<ScoreSystem>(GameSystemID::SCORE_SYSTEM).ResetScore();
    player->Revive();
    player->SetPosition(GameWorldConstants::playerSpawn);

    currentGameState = GameState::BEGIN_WAVE;
}



