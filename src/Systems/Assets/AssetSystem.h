//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once

#include "raylib.h"
#include <vector>
#include "../IGameSystem.h"
#include "SpriteID.h"
#include "TextureID.h"
#include "../../Utilities/utils.h"
#include "Sprite.h"

enum class EnemyID;

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


    private:
    void Run(GameWorld &world) override;
    void InitAssets();
    void LoadTex(TextureID id, const char* path);
    void DefineSprite (SpriteID id, TextureID textureID, Rectangle src);


    Font pixelFont;
    std::array<Texture2D, ToIndex(TextureID::COUNT)> textures{};
    std::array<Sprite, ToIndex(SpriteID::COUNT)> sprites{};

};


