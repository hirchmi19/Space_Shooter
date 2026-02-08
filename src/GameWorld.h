//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once

#include  "raylib.h"
#include <vector>
#include "Entities/Enemy.h"
#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include "Utilities/utils.h"
#include "Systems/GameSystemID.h"
#include "Systems/Assets/AssetSystem.h"
#include "Systems/Rendering/BackgroundSystem.h"
#include "Systems/Rendering/RenderSystem.h"

class Player;
class IGameSystem;
enum class SpriteID : uint32_t;


class GameWorld {

    public:
    GameWorld();
    ~GameWorld() = default;

    void RunGameplaySystems();
    void RunRenderSystems();

    void SpawnProjectile(SpriteID id, Vector2 spawnPosition);
    void SpawnEnemy(SpriteID id, Vector2 spawnPosition);
    const Player& GetPlayer() const;

    const Texture2D& GetTexture(TextureID id) const;
    const Sprite& GetSprite(SpriteID id) const;

    void RenderBackground() const;


    private:
   // Player player;

    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    std::vector<size_t> enemiesToRemove;
    std::vector<size_t> projectilesToRemove;

    std::array<std::unique_ptr<IGameSystem>, ToIndex(GameSystemID::COUNT)> gameSystems{};

    void CreateSystems();
    void AddSystem(std::unique_ptr<IGameSystem> system);

    const AssetSystem& GetAssetSystem() const;
    const BackgroundSystem& GetBackgroundSystem() const;

    void KillEntities();
    void KillEnemies();
    void KillProjectiles();

    void FindDeadEntities();
    void FindDeadProjectiles();
    void FindDeadEnemies();

};


