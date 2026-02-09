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


/**
 * Returns a Texture
 * \param id
 * \return
 */
const Texture2D& AssetSystem::GetTexture(const TextureID id) const {

    const size_t index = ToIndex(id);
    return textures[index];
}

/**
 * Returns a Sprite
 * \param id
 * \return
 */
const Sprite& AssetSystem::GetSprite(const SpriteID id) const {

    const size_t index = ToIndex(id);
    return sprites[index];
}

std::vector<const Sprite*> AssetSystem::GetPlayerSprites() const {

    return {
        &GetSprite(SpriteID::PLAYER_SHIP_LEFT),
        &GetSprite(SpriteID::PLAYER_SHIP_MIDDLE),
        &GetSprite(SpriteID::PLAYER_SHIP_RIGHT)
    };
}

//--------------------------------------------------------------------------


void AssetSystem::LoadTex(const TextureID id, const char* path) {

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

    LoadTex(TextureID::PLAYER_SHIP_CANVAS, "assets/SpaceShooterAssetPack_Ships.png");
    LoadTex(TextureID::ENEMY_SHIP_CANVAS, "assets/pico8_invaders_sprites_LARGE.png");
    LoadTex(TextureID::PROJECTILE_CANVAS, "assets/SpaceShooterAssetPack_Projectiles.png");

    //Player sprites
    DefineSprite(SpriteID::PLAYER_SHIP_MIDDLE, TextureID::PLAYER_SHIP_CANVAS, {8, 0, 8, 8});
    DefineSprite(SpriteID::PLAYER_SHIP_LEFT, TextureID::PLAYER_SHIP_CANVAS, {0, 0, 6, 8});
    DefineSprite(SpriteID::PLAYER_SHIP_RIGHT, TextureID::PLAYER_SHIP_CANVAS, {18, 0, 6, 8});

    DefineSprite(SpriteID::SMALL_ENEMY_SHIP, TextureID::PLAYER_SHIP_CANVAS, {73, 2, 7, 5});

    DefineSprite(SpriteID::PLAYER_PROJECTILE, TextureID::PLAYER_SHIP_CANVAS, {21, 44, 1, 4});
}

