//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include <array>
#include "raylib.h"
#include "../ISystem.h"
#include "SpriteID.h"
#include "TextureID.h"
#include "../../Utilities/utils.h"
#include "Sprite.h"

class AssetSystem : public ISystem {

    public:
    AssetSystem();
    ~AssetSystem() override;

    void InitSprites() override;

    const Texture2D& GetTexture(TextureID id) const;
    const Sprite& GetSprite(SpriteID id) const;

    private:
    void LoadTex(TextureID id, const char* path);
    void DefineSprite (SpriteID id, TextureID textureID, Rectangle src);

    std::array<Texture2D, ToIndex(TextureID::COUNT)> textures{};
    std::array<Sprite, ToIndex(SpriteID::COUNT)> sprites{};

};


