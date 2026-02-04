//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include <unordered_map>

#include "raylib.h"
#include "Sprite.h"
#include "SpriteID.h"
#include "TextureID.h"
#include "../../Helper/EnumHash.h"


class AssetSystem {

    public:
    AssetSystem();
    ~AssetSystem();

    void InitAssets();

    const Texture2D& GetTexture(TextureID texture) const;
    const Sprite& GetSprite(SpriteID texture) const;

    private:
    void LoadTex(TextureID id, const char* path);
    void DefineSprite (SpriteID id, TextureID textureID, Rectangle src);

    std::unordered_map<TextureID, Texture2D, EnumHash<TextureID>> textures;
    std::unordered_map<SpriteID, Sprite, EnumHash<SpriteID>> sprites;


};


