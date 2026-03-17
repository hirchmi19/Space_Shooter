//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include <vector>
#include "raylib.h"
#include "Assets.h"
#include "SpriteTable.h"
#include "../../game/IGameSystem.h"
#include "../../utils//utils.h"
#include "../../../include/locators/IAssetLocator.h"
#include "../../entities/projectiles/ProjectileType.h"



enum class PowerUpType;

class AssetSystem : public IAssetLocator, public IGameSystem {

    public:
    AssetSystem();
    ~AssetSystem() override;

    const Texture2D& GetTexture(TextureID id) const override;
    const Font& GetFont() const  override { return pixelFont; }

    std::vector<const Sprite*> GetPlayerSprite() const override
    {return

    {&PLAYER_SPRITE_TABLE[ToIndex(SpriteID::PLAYER_SHIP_MIDDLE)],
        &PLAYER_SPRITE_TABLE[ToIndex(SpriteID::PLAYER_SHIP_LEFT)],
        &PLAYER_SPRITE_TABLE[ToIndex(SpriteID::PLAYER_SHIP_RIGHT)]};}

    std::vector<const Sprite*> GetEnemySprites(const EnemyType& eType) const override
    {return {&ENEMY_SPRITE_TABLE[ToIndex(eType)].first, &ENEMY_SPRITE_TABLE[ToIndex(eType)].second};}

    std::vector<const Sprite*> GetProjectileSprites(const ProjectileType& pType) const override
    {return {&PROJECTILE_SPRITE_TABLE[ToIndex(pType)]};}

    std::vector<const Sprite *> GetPowerUpIcon(const PowerUpType& id) const override
    { return {&POWER_UP_ICON_SPRITE_TABLE[ToIndex(id)]}; }

    std::vector<const Sprite*> GetEffectSprite(const EffectID& id) const override
    {return {&EFFECT_SPRITE_TABLE[ToIndex(id)]}; }

    private:

    void Run() override;
    void Init() override;
    void LoadTex(TextureID id, const char* path);


    Font pixelFont = Font();
    std::array<Texture2D, ToIndex(TextureID::COUNT)> textures{};

};


