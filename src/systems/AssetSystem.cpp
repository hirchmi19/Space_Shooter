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

//--------------------------------------------------------------------------


void AssetSystem::LoadTex(const TextureID id, const char* path) {

    const size_t index = ToIndex(id);
    textures[index] = LoadTexture(path);
}

/**
 * Loads the textures and sets texture filters
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
}

void AssetSystem::Run() {

}

