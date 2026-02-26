//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once

#include <vector>
#include "raylib.h"
#include "AssetHeader/Assets.h"
#include "../IGameSystem.h"
#include "../../Utilities/utils.h"

enum class EnemyID : uint32_t;

class AssetSystem : public IGameSystem {

    public:
    AssetSystem();
    ~AssetSystem() override;


    const Texture2D& GetTexture(TextureID id) const;
    const Sprite& GetSprite(SpriteID id) const;
    const Font& GetFont() const { return pixelFont; }
    std::vector<const Sprite*> GetPlayerSprites() const;
    std::vector<const Sprite*> GetEnemySprites(EnemyID id) const;
    std::vector<const Sprite*> GetProjectileSprite(SpriteID id) const;
    std::vector <const Sprite*> GetExplosionSprites() const;


    private:
    void Init() override;
    void LoadTex(TextureID id, const char* path);
    void DefineSprite (SpriteID id, TextureID textureID, Rectangle src);


    Font pixelFont = Font();
    std::array<Texture2D, ToIndex(TextureID::COUNT)> textures{};
    std::array<Sprite, ToIndex(SpriteID::COUNT)> sprites{};

};


