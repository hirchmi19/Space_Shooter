//
// Created by Michael Hirsch on 04.02.26.
//

#include "GameWorld.h"

#include "external/glfw/src/cocoa_platform.h"
#include "Systems/Assets/AssetSystem.h"


GameWorld::GameWorld() {

    CreateSystems();
}

void GameWorld::RunGameplaySystems() const {


}

void GameWorld::RunRenderSystems() const {



}

const Player& GameWorld::GetPlayer() const {return player;}

void GameWorld::SpawnEnemy(SpriteID id, Vector2 spawnPosition) {

}

void GameWorld::SpawnProjectile(SpriteID id, Vector2 spawnPosition) {


}


void GameWorld::CreateSystems() {

    AddSystem(std::make_unique<AssetSystem>());
}

void GameWorld::AddSystem(std::unique_ptr<IGameSystem> system) {

    const GameSystemID id = system->GetSystemID();
    const size_t index = ToIndex(id);
    gameSystems[index] = std::move(system);
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
        if (enemy.IsAlive()) continue;
        enemiesToRemove.push_back(i);
    }
}

void GameWorld::FindDeadProjectiles() {

    for (size_t i = 0; i < projectiles.size(); ++i) {

        Projectile& projectile = projectiles[i];
        if (projectile.IsAlive()) continue;
        projectilesToRemove.push_back(i);
    }
}

void GameWorld::KillEnemies() {

    for (auto i = enemiesToRemove.rbegin(); i != enemiesToRemove.rend(); ++i) {

        enemies.erase(enemies.begin() + *i);
    }
}

void GameWorld::KillProjectiles() {

    for (auto i = projectilesToRemove.rbegin(); i != projectilesToRemove.rend(); ++i) {

        projectiles.erase(projectiles.begin() + *i);
    }
}





