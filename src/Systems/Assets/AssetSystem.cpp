//
// Created by Michael Hirsch on 04.02.26.
//

#include "AssetSystem.h"

#include <iostream>
#include <ostream>

#include "raylib.h"
#include "../../Entities/EntityType.h"
#include "Entities/Enemies/EnemyType.h"

AssetSystem::AssetSystem() : IGameSystem(GameSystemID::ASSET_SYSTEM, "ASSET_SYSTEM") {}


AssetSystem::~AssetSystem() {

    for (const auto& texture : textures) {

        UnloadTexture(texture);
    }

    std::cout << "CUSTOM TEXTURES UNLOADED SUCCESSFULLY" << std::endl;
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

/**
 * Returns the player sprites
 * \return
 */
std::vector<const Sprite*> AssetSystem::GetPlayerSprites() const {

    return {
        &GetSprite(SpriteID::PLAYER_SHIP_LEFT),
        &GetSprite(SpriteID::PLAYER_SHIP_MIDDLE),
        &GetSprite(SpriteID::PLAYER_SHIP_RIGHT)
    };
}

/**
 * Returns a projectile Sprite
 * \param eType
 * \return
 */
std::vector<const Sprite*> AssetSystem::GetProjectileSprite(const EntityType &eType) const {

    if (eType == EntityType::ENEMY_PROJECTILE) return {&GetSprite(SpriteID::ENEMY_PROJECTILE)};
    if (eType == EntityType::PLAYER_PROJECTILE) return {&GetSprite(SpriteID::PLAYER_PROJECTILE)};

    return {};
}

/**
 * Returns the enemy sprites of a specific enemy type
 * \param eType
 * \return
 */
std::vector<const Sprite *> AssetSystem::GetEnemySprites(const EntityType &eType) const {

    if (eType == EntityType::YELLOW_E) return {&GetSprite(SpriteID::YELLOW_ENEMY_0), &GetSprite(SpriteID::YELLOW_ENEMY_1)};
    if (eType == EntityType::RED_E) return {&GetSprite(SpriteID::RED_ENEMY_0), &GetSprite(SpriteID::RED_ENEMY_1)};
    if (eType == EntityType::BLACK_E) return {&GetSprite(SpriteID::BLACK_ENEMY_0), &GetSprite(SpriteID::BLACK_ENEMY_1)};

    return {};

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

/**
 * Loads the sprite sheets and cuts out the sprites
 */
void AssetSystem::Init() {

    //Load textures
    LoadTex(TextureID::PLAYER_SHIP_CANVAS,  ASSETS_PATH "SpaceShooterAssetPack_Ships.png");
    LoadTex(TextureID::ENEMY_CANVAS,        ASSETS_PATH "pico8_invaders_sprites_LARGE.png");
    LoadTex(TextureID::PROJECTILE_CANVAS,   ASSETS_PATH "SpaceShooterAssetPack_Projectiles.png");
    LoadTex(TextureID::EFFECT_CANVAS,       ASSETS_PATH "SpaceShooterAssetPack_Miscellaneous.png");

    // pixel font
    pixelFont = LoadFont(ASSETS_PATH "pixelFont.ttf");
    SetTextureFilter(pixelFont.texture, TEXTURE_FILTER_POINT);

    for (const auto& texture : textures) {

        SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    }

    //Player sprites
    DefineSprite(SpriteID::PLAYER_SHIP_MIDDLE, TextureID::PLAYER_SHIP_CANVAS, {8, 0, 8, 8});
    DefineSprite(SpriteID::PLAYER_SHIP_LEFT, TextureID::PLAYER_SHIP_CANVAS, {0, 0, 6, 8});
    DefineSprite(SpriteID::PLAYER_SHIP_RIGHT, TextureID::PLAYER_SHIP_CANVAS, {18, 0, 6, 8});

    //--------------------------------------------------------------------------

    //Enemy sprites
    DefineSprite(SpriteID::YELLOW_ENEMY_0, TextureID::ENEMY_CANVAS, {56, 0, 8, 8});
    DefineSprite(SpriteID::YELLOW_ENEMY_1, TextureID::ENEMY_CANVAS, {56, 8, 8, 7});

    DefineSprite(SpriteID::RED_ENEMY_0, TextureID::ENEMY_CANVAS, {40, 0, 8, 8});
    DefineSprite(SpriteID::RED_ENEMY_1, TextureID::ENEMY_CANVAS, {40, 8, 8 ,8});

    DefineSprite(SpriteID::BLACK_ENEMY_0, TextureID::ENEMY_CANVAS, {72, 0, 8, 8});
    DefineSprite(SpriteID::BLACK_ENEMY_1, TextureID::ENEMY_CANVAS, {72, 8, 8 ,8});

    //--------------------------------------------------------------------------

    //Projectile sprites
    DefineSprite(SpriteID::PLAYER_PROJECTILE, TextureID::ENEMY_CANVAS, {20, 42, 1, 4});
    DefineSprite(SpriteID::ENEMY_PROJECTILE, TextureID::ENEMY_CANVAS, {12, 42, 1, 4});

    //--------------------------------------------------------------------------



}

