//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once
#include <vector>

#include "Star.h"
#include "../IGameSystem.h"


//Constants for starfield background
constexpr uint32_t MAX_NUMBER_OF_STARS = 120;
//----------------------------------------------------------------------

class BackgroundSystem : public IGameSystem{

    public:
    BackgroundSystem();
    ~BackgroundSystem() override = default;

    void Run(GameWorld& world) override;
    void Render() const;

    private:
    void RespawnStar(size_t index);
    void InitStars();
    std::array<Star, MAX_NUMBER_OF_STARS> stars{};

};

