//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include <unordered_map>
#include <vector>

#include "raylib.h"
#include "Systems/Assets/SpriteID.h"
#include "Entities/Player.h"

class Entity;
class ISystem;
class EntityType;


class GameWorld {

    public:
    GameWorld();
    ~GameWorld();

    void RunSystems();
    void CreateProjectile(SpriteID id, Vector2 spawnPosition);
    void CreateEnemy(SpriteID id, Vector2 spawnPosition);
    Player& GetPlayer();

    private:
    Player player;
    std::unordered_map<EntityType, std::vector<std::unique_ptr<Entity>>> entities;
    std::vector<std::unique_ptr<ISystem>> systems;

    void CreateSystems();
    void AddEntity(std::unique_ptr<Entity> entity);
    void RemoveEntity(std::unique_ptr<Entity> entity);
};


