//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once

#include <vector>
#include "raylib.h"
#include "AssetHeader/Assets.h"
#include "../IGameSystem.h"
#include "../../Utilities/utils.h"
#include "SystemService/IAssetLocator.h"

enum class EnemyType : uint32_t;
enum class EntityType : uint32_t;

class AssetSystem : public IAssetLocator, public IGameSystem {

    public:
    AssetSystem();
    ~AssetSystem() override;


    const Texture2D& GetTexture(TextureID id) const override;
    const Sprite& GetSprite(SpriteID id) const override;
    const Font& GetFont() const  override { return pixelFont; }
    std::vector<const Sprite*> GetPlayerSprites() const override;
    std::vector<const Sprite*> GetEnemySprites(const EntityType &eType) const override;
    std::vector<const Sprite*> GetProjectileSprite(const EntityType &eType) const override;



    private:
    void Init() override;
    void LoadTex(TextureID id, const char* path);
    void DefineSprite (SpriteID id, TextureID textureID, Rectangle src);


    Font pixelFont = Font();
    std::array<Texture2D, ToIndex(TextureID::COUNT)> textures{};
    std::array<Sprite, ToIndex(SpriteID::COUNT)> sprites{};

};


