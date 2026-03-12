//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once

#include <cassert>
#include "GameState.h"
#include "utils/utils.h"
#include "GameSystemID.h"
#include "../systems/systems.h"
#include "../locators/SystemLocator.h"

class IGameSystem;
enum class SpriteID;

class GameWorld {

    public:
    GameWorld();
    ~GameWorld() = default;

    void RunGameplaySystems();
    void RunRenderSystem();

    const GameState& GetGameState() const { return currentGameState; };
    void EndWave() {currentGameState = GameState::END_WAVE;};

    private:

    std::array<std::shared_ptr<IGameSystem>, ToIndex(GameSystemID::COUNT)> gameSystems{};

    GameState currentGameState = GameState::BEGIN_WAVE;

    size_t transitionTimer{};
    bool timerStarted = false;

    void CreateSystems();
    void InitGameSystems();
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
