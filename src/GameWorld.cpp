//
// Created by Michael Hirsch on 04.02.26.
//

#include "GameWorld.h"

#include <cassert>

#include "Systems/Assets/AssetSystem.h"


GameWorld::GameWorld() {

    CreateSystems();
}

void GameWorld::RunGameplaySystems() {

    auto& sys = gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)];
    if (sys) sys->Run(*this);

}

void GameWorld::RunRenderSystems() {

    auto& sys = gameSystems[ToIndex(GameSystemID::RENDERER_SYSTEM)];
    if (sys) sys->Run(*this);

}


const Sprite& GameWorld::GetSprite(const SpriteID id) const {

    return GetAssetSystem().GetSprite(id);
}

const Texture2D& GameWorld::GetTexture(TextureID id) const {

    return GetAssetSystem().GetTexture(id);
}

void GameWorld::RenderBackground() const {

    GetBackgroundSystem().Render();
}


//const Player& GameWorld::GetPlayer() const {return player;}

void GameWorld::SpawnEnemy(SpriteID id, Vector2 spawnPosition) {

}

void GameWorld::SpawnProjectile(SpriteID id, Vector2 spawnPosition) {


}




void GameWorld::CreateSystems() {

    AddSystem(std::make_unique<AssetSystem>());
    AddSystem(std::make_unique<RenderSystem>());
    AddSystem(std::make_unique<BackgroundSystem>());
}

void GameWorld::AddSystem(std::unique_ptr<IGameSystem> system) {

    const GameSystemID id = system->GetSystemID();
    const size_t index = ToIndex(id);
    gameSystems[index] = std::move(system);
}

const AssetSystem& GameWorld::GetAssetSystem() const {

    const auto& ptr = gameSystems[ToIndex(GameSystemID::ASSET_SYSTEM)];

    assert(ptr && "AssetSystem is not initialized!");
    return static_cast<const AssetSystem&>(*ptr);
}

const BackgroundSystem &GameWorld::GetBackgroundSystem() const {

    const auto& ptr = gameSystems[ToIndex(GameSystemID::BACKGROUND_SYSTEM)];

    assert(ptr && "BackgroundSystem is not initialized!");
    return static_cast<const BackgroundSystem&>(*ptr);
}


void GameWorld::KillEntities() {

    KillEnemies();
    KillProjectiles();
}

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





