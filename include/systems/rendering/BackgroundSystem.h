//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once
#include <vector>

#include "Star.h"
#include "../../game/IGameSystem.h"


//Constants for starfield background
constexpr size_t MAX_NUMBER_OF_STARS = 120;
//----------------------------------------------------------------------

class BackgroundSystem : public IGameSystem{

    public:
    BackgroundSystem();
    ~BackgroundSystem() override = default;

    void Run() override;
    void Render();

    private:
    void RespawnStar(size_t index);
    void Init() override;
    std::array<Star, MAX_NUMBER_OF_STARS> stars{};

};

