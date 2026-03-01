//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include <vector>

#include "../Entities/EntityType.h"
#include "Entities/ProjectileType.h"
#include "Systems/Assets/AssetHeader/Sprite.h"
#include "Systems/Assets/AssetHeader/SpriteID.h"

enum class EnemyType : uint32_t;

struct IAssetLocator {

    virtual ~IAssetLocator() = default;
    virtual  std::vector<const Sprite*> GetPlayerSprites() const = 0;
    virtual  std::vector<const Sprite*> GetEnemySprites(const EnemyType &eType) const = 0;
    virtual  std::vector<const Sprite*> GetProjectileSprite(const ProjectileType& pType) const = 0;

    virtual const Texture2D& GetTexture(TextureID id) const = 0;
    virtual const Sprite& GetSprite(SpriteID id) const = 0;
    virtual const Font& GetFont() const = 0;

};


