//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include <memory>



#include "SpawnType.h"
#include "Entities/Entities.h"
#include "Systems/IGameSystem.h"
#include "SystemService/IEntityLocator.h"
#include "SpawnCommand.h"


enum class EntityType : uint32_t;


class EntitySystem : public IEntityLocator , public IGameSystem {

    public:

    EntitySystem();
    ~EntitySystem() override = default;

    void Run(GameWorld &world) override;
    void Init() override;

    void HandleInputs() const;
    bool PlayerAlive() const {return player->IsAlive();}
    void RevivePlayer() const { player->Revive();}


    void RequestEntitySpawn(const EntityType &eType, const SpawnType& sType, const Vector2 &pos) override;
    void ClearEntities();


    Player* getPlayer() const override {return player.get();};
    std::vector<Enemy>& GetEnemies() override {return enemies;};
    std::vector<Projectile>& GetProjectiles() override {return projectiles;};



    private:

    std::unique_ptr<Player> player;

    std::vector<Enemy> enemies{};
    std::vector<size_t> enemiesToRemove{};

    std::vector<Projectile> projectiles;
    std::vector<size_t> projectilesToRemove{};

    std::vector<SpawnCommand> spawnCmds{};

    void SpawnEntities();;
    void SpawnProjectile(const EntityType& eType, const Vector2& pos);
    void SpawnEnemy(const EntityType &eType, const Vector2& pos);

    void KillEntities();
    void KillEnemies();
    void KillProjectiles();

    void FindDeadEntities();
    void FindDeadProjectiles();
    void FindDeadEnemies();
};
