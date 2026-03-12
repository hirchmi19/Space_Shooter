//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once

#include <vector>
#include "raylib.h"
#include "../entities/projectiles/ProjectileType.h"
#include "../entities/enemies/EnemyType.h"
#include "systems/assets/EffectID.h"

struct Sprite;
enum class SpriteID;
enum class TextureID;
enum class PowerUpType;

struct IAssetLocator {

    virtual ~IAssetLocator() = default;
    virtual std::vector<const Sprite*> GetPlayerSprite() const = 0;
    virtual std::vector<const Sprite*> GetEnemySprites(const EnemyType &eType) const = 0;
    virtual std::vector<const Sprite*> GetProjectileSprites(const ProjectileType& pType) const = 0;
    virtual std::vector<const Sprite*> GetPowerUpIcon(const PowerUpType& id) const = 0;
    virtual std::vector<const Sprite*> GetEffectSprite(const EffectID& id) const = 0;

    virtual const Texture2D& GetTexture(TextureID id) const = 0;
    virtual const Font& GetFont() const = 0;

};


