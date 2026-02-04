//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include <unordered_map>
#include "Components/PositionComponent.h"


class GameWorld {

    public:
    using Entity = uint32_t;

    Entity CreateEntity(Entity entity);
    void DestroyEntity(Entity entity);

    template <typename T>
    void AddComponent(Entity e, T component);

    template <typename T>
    bool HasComponent (Entity e) const ;

    template <typename T>
    T& GetComponent (Entity e);

    private:
    Entity nextEntity = 1;

    std::unordered_map<Entity, PositionComponent> positions;
};


