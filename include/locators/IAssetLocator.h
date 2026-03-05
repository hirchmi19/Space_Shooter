//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once

#include <vector>
#include "raylib.h"
#include "../entities/projectiles/ProjectileType.h"
#include "../entities/enemies/EnemyType.h"

struct Sprite;
enum class SpriteID;
enum class TextureID;
enum class PowerUpType;

struct IAssetLocator {

    virtual ~IAssetLocator() = default;
    virtual  std::vector<const Sprite*> GetPlayerSprites() const = 0;
    virtual  std::vector<const Sprite*> GetEnemySprites(const EnemyType &eType) const = 0;
    virtual  std::vector<const Sprite*> GetProjectileSprite(const ProjectileType& pType) const = 0;
    virtual  std::vector<const Sprite*> GetPowerUpSprite(const PowerUpType& type) const = 0;
    virtual std::vector<const Sprite*> GetShieldSprite() const = 0;

    virtual const Texture2D& GetTexture(TextureID id) const = 0;
    virtual const Sprite& GetSprite(SpriteID id) const = 0;
    virtual const Font& GetFont() const = 0;

};


