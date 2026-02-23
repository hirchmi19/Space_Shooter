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
#include "../Systems/Scoring/ScoreSystem.h"
#include "../Systems/Waving/FormationSlot.h"
#include "../Systems/Waving/WaveSystem.h"

struct Explosion;
class Player;
class IGameSystem;
enum class SpriteID : uint32_t;

class GameWorld {

    public:
    GameWorld();
    ~GameWorld() = default;

    void RunGameplaySystems();
    void RunRenderSystem();

    void SpawnPlayerProjectile(const Vector2& playerPosition);
    void SpawnEnemy(const EnemyID& id, const Vector2& spawnPosition);
    void SpawnEnemyProjectile(const Vector2& enemyPosition);


    const Player& GetPlayer() const {return player;};
    Player& GetPlayer() {return player;};

    const Texture2D& GetTexture(const TextureID& id) const {return GetAssetSystem().GetTexture(id);};
    const Sprite& GetSprite(const SpriteID& id) const { return GetAssetSystem().GetSprite(id);};
    const Font& GetFont() const {return GetAssetSystem().GetFont();};

    const std::vector<const Sprite*> GetEnemySprites(const EnemyID id) const {return {GetAssetSystem().GetEnemySprites(id)};};
    const std::vector<const Sprite*> GetExplosionSprites() const {return GetAssetSystem().GetExplosionSprites();};

    const GameState& GetGameState() const { return currentGameState; };
    void EndWave() {currentGameState = GameState::END_WAVE;};

    int GetWaveCounter() const { return GetWaveSystem().GetWaveCounter(); };
    const uint32_t GetHighScore() const { return GetScoreSystem().GetHighScore(); };

    void RenderBackground() const { GetBackgroundSystem().Render();};

    std::vector<Projectile>& GetProjectiles() {return projectiles;};
    std::vector<Enemy>& GetEnemies() {return enemies;};


    private:

    Player player;

    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    std::vector<size_t> enemiesToRemove;
    std::vector<size_t> projectilesToRemove;

    std::array<std::unique_ptr<IGameSystem>, ToIndex(GameSystemID::COUNT)> gameSystems{};

    GameState currentGameState = GameState::BEGIN_WAVE;

    TimerComponent waveTimer;
    bool timerStarted = false;

    void CreateSystems();
    void AddSystem(std::unique_ptr<IGameSystem> system);

    const AssetSystem& GetAssetSystem() const;
    const BackgroundSystem& GetBackgroundSystem() const;
    RenderSystem& GetRenderSystem() const;
    WaveSystem& GetWaveSystem() const;
    ScoreSystem& GetScoreSystem() const;

    void KillEntities();
    void KillEnemies();
    void KillProjectiles();

    void Restart();
    void ClearEntities();

    void FindDeadEntities();
    void FindDeadProjectiles();
    void FindDeadEnemies();

};


