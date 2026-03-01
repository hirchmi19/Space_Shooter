//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include <memory>

#include "Systems/IGameSystem.h"
#include "Locators/IEntityLocator.h"
#include "SpawnEnemyCommand.h"
#include "SpawnProjectileCommand.h"
#include "Entities/Entities.h"


class EntitySystem : public IEntityLocator, public IGameSystem {

    public:

    EntitySystem();
    ~EntitySystem() override = default;

    void Run() override;
    void Init() override;

    void HandleInputs() const;
    bool PlayerAlive() const {return player->IsAlive();}

    void SpawnProjectile(const ProjectileType &pType, const Vector2& pos, bool isPlayerProjectile) override;
    void SpawnEnemy(const EnemyType &eType, const Vector2 &spawnPos) override;
    void ClearEntities();

    Player* GetPlayer() const override {return player.get();};
    std::vector<Enemy>& GetEnemies() override {return enemies;};
    std::vector<Projectile>& GetProjectiles() override {return projectiles;};


    private:

    std::unique_ptr<Player> player;

    std::vector<Enemy> enemies{};
    std::vector<size_t> enemiesToRemove{};

    std::vector<Projectile> projectiles;
    std::vector<size_t> projectilesToRemove{};

    void KillEntities();
    void KillEnemies();
    void KillProjectiles();

    void FindDeadEntities();
    void FindDeadProjectiles();
    void FindDeadEnemies();

    void RequestEntityRemoval(const EntityType&, size_t value) override;
};
