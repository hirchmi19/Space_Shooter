//
// Created by Michael Hirsch on 04.02.26.
//

#include "AssetSystem.h"
#include "raylib.h"

AssetSystem::AssetSystem() : IGameSystem(GameSystemID::ASSET_SYSTEM) {

    InitSprites();
}


AssetSystem::~AssetSystem() {

    for (const Texture2D texture : textures) {
        UnloadTexture(texture);
    }
}


void AssetSystem::LoadTex(const TextureID id, const char *path) {

    const size_t index = ToIndex(id);
    textures[index] = LoadTexture(path);
}

void AssetSystem::DefineSprite(const SpriteID id, const TextureID textureID, const Rectangle src) {

    const size_t index = ToIndex(id);
    sprites[index] = Sprite{textureID, src};
}

void AssetSystem::Run(GameWorld &world) {

    //NOTHING
}

/**
 * Loads the sprite sheets and cuts out the sprites
 */
void AssetSystem::InitSprites() {

    LoadTex(TextureID::BACKGROUND_ATLAS, "assets/SpaceShooterAssetPack_BackGrounds.png");
    LoadTex(TextureID::SHIP_ATLAS, "assets/SpaceShooterAssetPack_Ships.png");
    LoadTex(TextureID::PROJECTILE_ATLAS, "assets/SpaceShooterAssetPack_Projectiles.png");

    DefineSprite(SpriteID::BACKGROUND_STARS, TextureID::BACKGROUND_ATLAS, {0, 0, 128, 256});

    DefineSprite(SpriteID::PLAYER_SHIP, TextureID::SHIP_ATLAS, {32, 64, 16, 16});
    DefineSprite(SpriteID::SMALL_ENEMY_SHIP, TextureID::SHIP_ATLAS, {73, 2, 7, 5});

    DefineSprite(SpriteID::PLAYER_PROJECTILE, TextureID::PROJECTILE_ATLAS, {19, 3, 1, 3});
}

/**
 * Returns a Texture
 * \param id
 * \return
 */
Texture2D AssetSystem::GetTexture(const TextureID id) const {

    const size_t index = ToIndex(id);
    return textures[index];
}

/**
 * Returns a Sprite
 * \param id
 * \return
 */
 Sprite AssetSystem::GetSprite(const SpriteID id) const {

    const size_t index = ToIndex(id);
    return sprites[index];
}
