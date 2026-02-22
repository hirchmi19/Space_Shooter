//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once

#include  "raylib.h"
#include <vector>

#include "GameState.h"
#include "../Constants/GameConstants.h"
#include "../Entities/Enemy.h"
#include "../Entities/Player.h"
#include "../Entities/Projectile.h"
#include "../Utilities/utils.h"
#include "../Systems/GameSystemID.h"
#include "../Systems/Assets/AssetSystem.h"
#include "../Systems/Rendering/BackgroundSystem.h"
#include "../Systems/Rendering/RenderSystem.h"
#include "../Systems/Waving/FormationSlot.h"
#include "../Systems/Waving/WaveSystem.h"

class Player;
class IGameSystem;
enum class SpriteID : uint32_t;

//Constants for Player Spawn
constexpr float playerSpawnX = GameConstants::SCREEN_WIDTH / 2.0f - 8; // minus texture width
constexpr float playerSpawnY = GameConstants::SCREEN_HEIGHT - 100.0f; // minus an offset so player isn't at the bottom

constexpr Vector2 playerSpawn = {playerSpawnX, playerSpawnY};
constexpr Vector2 playerSize = {8, 7}; // player texture values
//----------------------------------------------------------------------

class GameWorld {

    public:
    GameWorld();
    ~GameWorld() = default;

    void RunGameplaySystems();
    void RunRenderSystems();

    void SpawnPlayerProjectile(const Vector2& playerPosition);

    void SpawnEnemy(const std::vector<const Sprite*>& sprites, const Vector2& spawnPosition);
    void SpawnEnemyProjectile(const Vector2& enemyPosition);

    const Player& GetPlayer() const;
    Player& GetPlayer();

    const Texture2D& GetTexture(TextureID id) const;
    const Sprite& GetSprite(SpriteID id) const;
    const Font& GetFont() const {return GetAssetSystem().GetFont();};
    const std::vector<const Sprite*> GetEnemySprites(EnemyID id) const;
    const GameState& GetGameState() const { return currentGameState; };

    void RenderBackground() const;

    std::vector<Projectile>& GetProjectiles() {return projectiles;};
    std::vector<Enemy>& GetEnemies() {return enemies;};
    bool AllEnemiesKilled() const {return enemies.empty();};

    private:

    Player player;

    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    std::vector<size_t> enemiesToRemove;
    std::vector<size_t> projectilesToRemove;

    std::array<std::unique_ptr<IGameSystem>, ToIndex(GameSystemID::COUNT)> gameSystems{};

    GameState currentGameState = GameState::IN_GAME;

    void CreateSystems();
    void AddSystem(std::unique_ptr<IGameSystem> system);

    const AssetSystem& GetAssetSystem() const;
    const BackgroundSystem& GetBackgroundSystem() const;
    const RenderSystem& GetRenderSystem() const;
    WaveSystem& GetWaveSystem() const;

    void KillEntities();
    void KillEnemies();
    void KillProjectiles();

    void Restart();

    void FindDeadEntities();
    void FindDeadProjectiles();
    void FindDeadEnemies();

};


