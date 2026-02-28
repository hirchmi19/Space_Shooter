//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include <cstdint>


enum class EntityType : uint32_t;

struct IScoreLocator {

    virtual ~IScoreLocator() = default;
    virtual uint32_t GetEnemyScore(const EntityType& id) = 0;
    virtual void AddHighScore(uint32_t score) = 0;
    virtual void ResetScore() = 0;

};

