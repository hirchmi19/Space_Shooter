//
// Created by Michael Hirsch on 04.02.26.
//

#include "GameWorld.h"
#include <cassert>
#include <iostream>
#include <ostream>

#include "../Systems/Assets/AssetSystem.h"
#include "../Systems/Movement/MovementSystem.h"
#include  "../Systems/Rendering/RenderSystem.h"
#include "../Systems/Waving/WaveSystem.h"

GameWorld::GameWorld() : player(playerSpawn, playerSize, GetAssetSystem().GetPlayerSprites()) {

    CreateSystems();

}

/**
 * Runs all gameplay systems by calling their run/update functions
 */
void GameWorld::RunGameplaySystems() {

    player.HandleInput(*this);

    const auto& bgSys = gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)];
    const auto& moveSys = gameSystems[ToIndex(GameSystemID::MOVEMENT_SYSTEM)];
    const auto& waveSys = gameSystems[ToIndex(GameSystemID::WAVE_SYSTEM)];

    if (bgSys) bgSys->Run(*this);
    if (moveSys) moveSys->Run(*this);
    if (waveSys) waveSys->Run(*this);

    FindDeadEntities();
    KillEntities();

}

/**
 * Renders EVERYTHING
 */
void GameWorld::RunRenderSystems() {

    auto& renderSys = gameSystems[ToIndex(GameSystemID::RENDERER_SYSTEM)];
    if (renderSys) renderSys->Run(*this);

}

/**
 * Returns a Sprite struct
 * \param id
 * \return
 */
const Sprite& GameWorld::GetSprite(const SpriteID id) const {

    return GetAssetSystem().GetSprite(id);
}

/**
 * Returns a Raylib Texture
 * \param id
 * \return
 */
const Texture2D& GameWorld::GetTexture(TextureID id) const {

    return GetAssetSystem().GetTexture(id);
}


/**
 * Returns all enemy sprites by a given ID
 * \param id
 * \return
 */
const std::vector<const Sprite *> GameWorld::GetEnemySprites(const EnemyID id) const {

    return {GetAssetSystem().GetEnemySprites(id)};

}

/**
 * Calls the render function of the background system
 */
void GameWorld::RenderBackground() const {

    GetBackgroundSystem().Render();
}

/**
 * Returns a  read-only reference to the player
 * \return
 */
const Player& GameWorld::GetPlayer() const {return player;}

/**
 * Returns a reference to the player
 * \return
 */
Player& GameWorld::GetPlayer() { return player; }

/**
 * Creates an Enemy
 * \param sprites
 * \param spawnPosition
 */
void GameWorld::SpawnEnemy(const std::vector<const Sprite *> &sprites, const Vector2 &spawnPosition) {


    const Vector2 size = {sprites[0]->src.width, sprites[0]->src.height};

    enemies.emplace_back(RenderComponent{sprites, size},
        CombatComponent{1,
            Rectangle{spawnPosition.x, spawnPosition.y,
            size.x * RenderConstants::ENEMY_SCALING,
            size.y * RenderConstants::ENEMY_SCALING}
        });
}

/**
 * Creates a Projectile
 * \param playerPosition
 */
void GameWorld::SpawnPlayerProjectile(const Vector2& playerPosition) {

    const Sprite& projectileSprite = GetAssetSystem().GetSprite(SpriteID::PLAYER_PROJECTILE);

    const Vector2 projectileSize {projectileSprite.src.width,projectileSprite.src.height};
    const float playerWidth = player.GetSize().x * RenderConstants::PLAYER_SCALING;
    const Vector2 spawnPosition {playerPosition.x + (playerWidth * 0.5f) - (projectileSize.x * 0.5f),playerPosition.y};
    constexpr int32_t speed = -1;

    const auto sprites = GetAssetSystem().GetProjectileSprite(SpriteID::PLAYER_PROJECTILE);

    projectiles.emplace_back(
        Movement1D{spawnPosition, speed},
        RenderComponent{sprites, projectileSize},
        CombatComponent{1,Rectangle{
                spawnPosition.x,
                spawnPosition.y,
                projectileSize.x * RenderConstants::PROJECTILE_SCALING,
                projectileSize.y * RenderConstants::PROJECTILE_SCALING
            }
        }
    );
}

void GameWorld::SpawnEnemyProjectile(const Vector2 &enemyPosition) {

    const Sprite& projectileSprite = GetAssetSystem().GetSprite(SpriteID::ENEMY_PROJECTILE);

    const Vector2 projectileSize {projectileSprite.src.width,projectileSprite.src.height};
    const float playerWidth = player.GetSize().x * RenderConstants::ENEMY_SCALING;
    const Vector2 spawnPosition {enemyPosition.x + (playerWidth * 0.5f) - (projectileSize.x * 0.5f),enemyPosition.y};
    constexpr int32_t speed = 1;

    const auto sprites = GetAssetSystem().GetProjectileSprite(SpriteID::ENEMY_PROJECTILE);

    projectiles.emplace_back(
        Movement1D{spawnPosition, speed},
        RenderComponent{sprites, projectileSize},
        CombatComponent{1,Rectangle{
                spawnPosition.x,
                spawnPosition.y,
                projectileSize.x * RenderConstants::PROJECTILE_SCALING,
                projectileSize.y * RenderConstants::PROJECTILE_SCALING
            }
        }
    );


}

//--------------------------------------------------------------------------

/**
 * Inits all game systems
 */
void GameWorld::CreateSystems() {

    AddSystem(std::make_unique<AssetSystem>());
    AddSystem(std::make_unique<RenderSystem>());
    AddSystem(std::make_unique<BackgroundSystem>());
    AddSystem(std::make_unique<MovementSystem>());
    AddSystem(std::make_unique<WaveSystem>());

    enemies.reserve(44);
    projectiles.reserve(25);
}

/**
 * Adds a system and gives GameWorld the ownership
 * \param system
 */
void GameWorld::AddSystem(std::unique_ptr<IGameSystem> system) {

    const GameSystemID id = system->GetSystemID();
    const size_t index = ToIndex(id);
    gameSystems[index] = std::move(system);
}

/**
* Returns a read-only reference to the asset system
 * \return
 */
const AssetSystem& GameWorld::GetAssetSystem() const {

    const auto& ptr = gameSystems[ToIndex(GameSystemID::ASSET_SYSTEM)];

    //assert(ptr && "AssetSystem is not initialized!");
    return static_cast<const AssetSystem&>(*ptr);
}

/**
 * Returns a read-only reference to the background system
 * \return
 */
const BackgroundSystem& GameWorld::GetBackgroundSystem() const {

    const auto& ptr = gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)];

    assert(ptr && "BackgroundSystem is not initialized!");
    return static_cast<const BackgroundSystem&>(*ptr);
}

/**
 * Removes all enemies and projectiles from the collection respectively GameWorld
 */
void GameWorld::KillEntities() {        //TODO: create an separate entity system for entity creation and destruction

    KillEnemies();
    KillProjectiles();
}

/**
 * Finds all enemies and projectiles which are marked for death and adds them to their respective removal queues
 */
void GameWorld::FindDeadEntities() {

    FindDeadEnemies();
    FindDeadProjectiles();

}


void GameWorld::FindDeadEnemies() {

    for (size_t i = 0; i < enemies.size(); ++i) {

        Enemy& enemy = enemies[i];
        if (enemy.combat.IsAlive()) continue;
        enemiesToRemove.push_back(i);
    }
}

void GameWorld::FindDeadProjectiles() {

    for (size_t i = 0; i < projectiles.size(); ++i) {

        Projectile& projectile = projectiles[i];
        if (projectile.combat.IsAlive()) continue;
        projectilesToRemove.push_back(i);
    }
}

void GameWorld::KillEnemies() {

    for (auto i = enemiesToRemove.rbegin(); i != enemiesToRemove.rend(); ++i) {

        enemies.erase(enemies.begin() + *i);
    }

    enemiesToRemove.clear();
}

void GameWorld::KillProjectiles() {

    for (auto i = projectilesToRemove.rbegin(); i != projectilesToRemove.rend(); ++i) {

        projectiles.erase(projectiles.begin() + *i);
    }

    projectilesToRemove.clear();
}


