//
// Created by Michael Hirsch on 04.02.26.
//

#include "AssetSystem.h"

AssetSystem::AssetSystem() = default;

AssetSystem::~AssetSystem() {

    for (const auto& [id, texture] : textures ) {
        UnloadTexture(texture);
    }
}


void AssetSystem::LoadTex(TextureID id, const char *path) {

    textures.emplace(id, LoadTexture(path));
}

void AssetSystem::DefineSprite(SpriteID id, TextureID textureID, Rectangle src) {

    sprites.emplace(id, Sprite{textureID, src});
}

void AssetSystem::InitAssets() {

    LoadTex(TextureID::BACKGROUND_ATLAS, "assets/SpaceShooterAssetPack_BackGrounds.png");
    LoadTex(TextureID::SHIP_ATLAS, "assets/SpaceShooterAssetPack_Ships.png");
    LoadTex(TextureID::PROJECTILE_ATLAS, "assets/SpaceShooterAssetPack_Projectiles.png");

    DefineSprite(SpriteID::BACKGROUND_STARS, TextureID::BACKGROUND_ATLAS, {0, 0, 128, 256});

    DefineSprite(SpriteID::PLAYER_SHIP, TextureID::SHIP_ATLAS, {32, 64, 16, 16});
    DefineSprite(SpriteID::SMALL_ENEMY_SHIP, TextureID::SHIP_ATLAS, {73, 2, 7, 5});

    DefineSprite(SpriteID::PLAYER_PROJECTILE, TextureID::PROJECTILE_ATLAS, {19, 3, 1, 3});
}


const Texture2D& AssetSystem::GetTexture(TextureID id) const {
    return textures.at(id);
}

const Sprite& AssetSystem::GetSprite(SpriteID id) const {
    return sprites.at(id);
}
