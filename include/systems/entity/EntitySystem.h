//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include <memory>
#include <vector>

#include "../../game/IGameSystem.h"
#include "../../locators/IEntityLocator.h"
#include "../include/entities//Entities.h"
#include "entities/Explosion.h"
#include "entities/PowerUp.h"
#include "entities/Shield.h"
#include "utils/utils.h"


enum class PowerUpType;
enum class EnemyType;

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
    void SpawnPowerUp(PowerUpType type, const Vector2& spawnPos) override;
    void ClearEntities();
    void LvlProjectiles() override;
    void Reset() override { projctileHp = 1, shield.hp = 1;}

    Player* GetPlayer() const override {return player.get();};
    Shield& GetShield() override {return shield;};
    std::vector<Enemy>& GetEnemies() override {return enemies;};
    std::vector<Projectile>& GetProjectiles() override {return projectiles;};
    std::vector<PowerUp>& GetPowerUps() override {return powerUps;};
    std::vector<Explosion>& GetExplosions() override { return explosions;};
    float GetProjectileHp() override {return projctileHp;};

    void SpawnExplosion(const Vector2& pos) override;


    private:

    std::unique_ptr<Player> player;

    std::vector<Enemy> enemies{};
    std::vector<size_t> deadEnemies{};

    std::vector<Projectile> projectiles{};
    std::vector<size_t> deadProjectiles{};

    std::vector<PowerUp> powerUps{};
    std::vector<size_t> deadPowerUps{};

    std::vector<Explosion> explosions{};

    Shield shield;
    float projctileHp = 1;

    void KillEntities();
    void KillEnemies();
    void KillProjectiles();
    void KillPowerUps();

    void FindDeadEntities();
    void FindDeadProjectiles();
    void FindDeadEnemies();
    void FindDeadPowerUps();

    void RequestEntityRemoval(const EntityType&, size_t value) override;
    std::vector<size_t>& GetRemovalQ(const EntityType& type);
};
