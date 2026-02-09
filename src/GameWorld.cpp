//
// Created by Michael Hirsch on 04.02.26.
//

#include "GameWorld.h"
#include <cassert>
#include "Systems/Assets/AssetSystem.h"

GameWorld::GameWorld() : player(playerSpawn, playerSize, GetAssetSystem().GetPlayerSprites()) {

    CreateSystems();

}

/**
 * Runs all gameplay systems by calling their run/update functions
 */
void GameWorld::RunGameplaySystems() {

    auto& sys = gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)];
    if (sys) sys->Run(*this);

}

/**
 * Renders EVERYTHING
 */
void GameWorld::RunRenderSystems() {

    auto& sys = gameSystems[ToIndex(GameSystemID::RENDERER_SYSTEM)];
    if (sys) sys->Run(*this);

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
 * Creates an Enemy struct
 * \param id
 * \param spawnPosition
 */
void GameWorld::SpawnEnemy(SpriteID id, Vector2 spawnPosition) {

}

/**
 * Creates a Projectile struct
 * \param id
 * \param spawnPosition
 */
void GameWorld::SpawnProjectile(SpriteID id, Vector2 spawnPosition) {

}

//--------------------------------------------------------------------------


/**
 * Inits all game systems
 */
void GameWorld::CreateSystems() {

    AddSystem(std::make_unique<AssetSystem>());
    AddSystem(std::make_unique<RenderSystem>());
    AddSystem(std::make_unique<BackgroundSystem>());
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
void GameWorld::KillEntities() {

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





