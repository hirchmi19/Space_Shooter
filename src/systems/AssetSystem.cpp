//
// Created by Michael Hirsch on 04.02.26.
//


#include <iostream>
#include <ostream>
#include "raylib.h"
#include "entities/enemies/EnemyType.h"
#include "systems/assets/SpriteID.h"
#include "systems/assets/TextureID.h"
#include "systems/assets/AssetSystem.h"

#include "entities/PowerUpType.h"

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
 * \param pType
 * \return
 */
std::vector<const Sprite*> AssetSystem::GetProjectileSprite(const ProjectileType& pType) const {

    if (pType == ProjectileType::BASE_ENEMY) return {&GetSprite(SpriteID::ENEMY_PROJECTILE)}; // this is so bad...
    if (pType == ProjectileType::ARROW) return {&GetSprite(SpriteID::ARROW)};
    if (pType == ProjectileType::GLAIVE) return {&GetSprite(SpriteID::GLAIVE)};
    if (pType == ProjectileType::ROCKET) return {&GetSprite(SpriteID::ROCKET)};

    return {&GetSprite(SpriteID::PLAYER_PROJECTILE)};

    return {};
}

/**
 * Returns the enemy sprites of a specific enemy type
 * \param eType
 * \return
 */
std::vector<const Sprite *> AssetSystem::GetEnemySprites(const EnemyType& eType) const {

    if (eType == EnemyType::YELLOW_E)return {&GetSprite(SpriteID::YELLOW_ENEMY_0),
        &GetSprite(SpriteID::YELLOW_ENEMY_1)};
    if (eType == EnemyType::RED_E)return {&GetSprite(SpriteID::RED_ENEMY_0),
        &GetSprite(SpriteID::RED_ENEMY_1)};
    if (eType == EnemyType::BLACK_E)return {&GetSprite(SpriteID::BLACK_ENEMY_0),
        &GetSprite(SpriteID::BLACK_ENEMY_1)};

    return {};

}

std::vector<const Sprite*> AssetSystem::GetPowerUpSprite(const PowerUpType &type) const {

    if (type == PowerUpType::LEVEL_UP) return {&GetSprite(SpriteID::ARROW_UP)};
    if (type == PowerUpType::SHIELD) return {&GetSprite(SpriteID::SHIELD_ICON)};

    const auto& prType = GetProjectileType(type);
    return {GetProjectileSprite(prType)};

    return {};
}

std::vector<const Sprite *> AssetSystem::GetShieldSprite() const {

    return {&GetSprite(SpriteID::SHIELD_EFFECT)};
}

ProjectileType AssetSystem::GetProjectileType(const PowerUpType &type) const {

    if (type == PowerUpType::COUNT || type == PowerUpType::NONE) return ProjectileType::NONE;

    if (type == PowerUpType::ARROW) return ProjectileType::ARROW; // i am crying...
    if (type == PowerUpType::GLAIVE) return ProjectileType::GLAIVE;
    if (type == PowerUpType::ROCKET) return ProjectileType::ROCKET;
    if (type == PowerUpType::SPLIT) return ProjectileType::SPLIT;
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
    DefineSprite(SpriteID::ROCKET, TextureID::PROJECTILE_CANVAS,{42, 25, 4, 5});
    DefineSprite(SpriteID::ARROW, TextureID::PROJECTILE_CANVAS,{10, 9, 3, 6});
    DefineSprite(SpriteID::GLAIVE, TextureID::PROJECTILE_CANVAS,{33, 9, 6, 4});

    //--------------------------------------------------------------------------

    //Power-ups
    DefineSprite(SpriteID::ARROW_UP, TextureID::EFFECT_CANVAS,{16, 8, 8, 8});
    DefineSprite(SpriteID::SHIELD_ICON, TextureID::EFFECT_CANVAS,{24, 0, 8, 8});
    DefineSprite(SpriteID::SHIELD_EFFECT, TextureID::EFFECT_CANVAS,{88, 32, 16, 16});

    //--------------------------------------------------------------------------

    //ui

    DefineSprite(SpriteID::SHIELD_UI, TextureID::EFFECT_CANVAS,{2, 26, 5, 5});
    DefineSprite(SpriteID::SPLIT_SHOT, TextureID::EFFECT_CANVAS,{24, 8, 8, 8});

    //--------------------------------------------------------------------------

    DefineSprite(SpriteID::EXPLOSION, TextureID::EFFECT_CANVAS,{80, 48, 8, 8});


}

void AssetSystem::Run() {

}

