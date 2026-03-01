//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once

#include <cassert>

#include  "raylib.h"
#include <vector>

#include "GameState.h"
#include "../Utilities/utils.h"
#include "../Systems/GameSystemID.h"
#include "../Systems/Scoring/ScoreSystem.h"
#include "../Systems/Waving/WaveSystem.h"
#include "Locators/SystemLocator.h"

class IGameSystem;
enum class SpriteID : uint32_t;

class GameWorld {

    public:
    GameWorld();
    ~GameWorld() = default;

    void RunGameplaySystems();
    void RunRenderSystem();

    const GameState& GetGameState() const { return currentGameState; };
    void EndWave() {currentGameState = GameState::END_WAVE;};

    int GetWaveCounter() { return GetGameSystemStatic<WaveSystem>(GameSystemID::WAVE_SYSTEM).GetWaveCounter(); };
    uint32_t GetHighScore() { return GetGameSystemStatic<ScoreSystem>(GameSystemID::SCORE_SYSTEM).GetHighScore(); };


    private:

    std::array<std::shared_ptr<IGameSystem>, ToIndex(GameSystemID::COUNT)> gameSystems{};

    GameState currentGameState = GameState::BEGIN_WAVE;

    TimerComponent transitionTimer;
    bool timerStarted = false;

    void CreateSystems();
    void InitGameSystems() const;
    void AddSystem(std::shared_ptr<IGameSystem> system);

    void Restart();

    template<typename T>
       T& GetGameSystemStatic(const GameSystemID& id) {
        const auto& ptr = gameSystems[ToIndex(id)];
        assert(ptr && "GameSystem is not initialized");
        return static_cast<T&>(*ptr);
    }

    /*    template<typename T>
     T& GetGameSystemDynamic(const GameSystemID& id) {

        const auto& ptr = gameSystems[ToIndex(id)];
        assert(ptr && "GameSystem is not initialized");
        return dynamic_cast<T&>(*ptr);
    }*/


};


