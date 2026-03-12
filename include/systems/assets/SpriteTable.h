#pragma once
#include <array>

#include "EffectID.h"
#include "Sprite.h"
#include "SpriteID.h"
#include "TextureID.h"
#include "entities/PowerUpType.h"
#include "entities/enemies/EnemyType.h"
#include "entities/projectiles/ProjectileType.h"
#include "utils/utils.h"

inline constexpr std::array PLAYER_SPRITE_TABLE = {

    Sprite{SpriteID::PLAYER_SHIP_MIDDLE, TextureID::PLAYER_SHIP_CANVAS, Rectangle{8, 0, 8, 8}},
    Sprite{SpriteID::PLAYER_SHIP_LEFT, TextureID::PLAYER_SHIP_CANVAS, Rectangle{0, 0, 6, 8}},
    Sprite{SpriteID::PLAYER_SHIP_RIGHT, TextureID::PLAYER_SHIP_CANVAS, Rectangle{18, 0, 6, 8}}

};

inline constexpr std::array<std::pair<Sprite, Sprite>, ToIndex(EnemyType::COUNT)> ENEMY_SPRITE_TABLE = {{

    {Sprite{SpriteID::YELLOW_ENEMY_0, TextureID::ENEMY_CANVAS, Rectangle{56, 0, 8, 8}},
        Sprite{SpriteID::YELLOW_ENEMY_1, TextureID::ENEMY_CANVAS, Rectangle{56, 8, 8, 7}}},// YELLOW_ENEMY

    {Sprite{SpriteID::RED_ENEMY_0, TextureID::ENEMY_CANVAS, Rectangle{40, 0, 8, 8}},
        Sprite{SpriteID::RED_ENEMY_1, TextureID::ENEMY_CANVAS, Rectangle{40, 8, 8, 8}}},  // RED_ENEMY

    {Sprite{SpriteID::BLACK_ENEMY_0, TextureID::ENEMY_CANVAS, Rectangle{72, 0, 8, 8}},
        Sprite{SpriteID::BLACK_ENEMY_1, TextureID::ENEMY_CANVAS, Rectangle{72, 8, 8, 8}}},  // BLACK_ENEMY
}};


inline constexpr std::array<Sprite, ToIndex(ProjectileType::COUNT)> PROJECTILE_SPRITE_TABLE = {

    Sprite{SpriteID::BASE_PLAYER_PROJECTILE, TextureID::PROJECTILE_CANVAS, Rectangle{19, 3, 1, 3}},
    Sprite{SpriteID::BASE_ENEMY_PROJECTILE, TextureID::PROJECTILE_CANVAS, Rectangle{11, 4, 1, 2}},
    Sprite{SpriteID::ARROW, TextureID::PROJECTILE_CANVAS, Rectangle{10, 9, 3, 6}},
    Sprite{SpriteID::ROCKET, TextureID::PROJECTILE_CANVAS, Rectangle{42, 25, 4, 5}},
    Sprite{SpriteID::GLAIVE, TextureID::PROJECTILE_CANVAS, Rectangle{33, 9, 6, 4}},
};

inline constexpr std::array POWER_UP_ICON_SPRITE_TABLE = {

    Sprite{}, // dummy entry to skip NONE entry
    Sprite{SpriteID::ARROW_LEVEL_UP, TextureID::EFFECT_CANVAS, Rectangle{16, 8, 8, 8}},
    Sprite{SpriteID::SHIELD_ICON, TextureID::EFFECT_CANVAS, Rectangle{24, 0, 8, 8}},
    Sprite{SpriteID::RANDOM_PROJECTILE, TextureID::PROJECTILE_CANVAS, Rectangle{2, 33, 5, 5}},
};

inline constexpr std::array<Sprite, ToIndex(EffectID::COUNT)> EFFECT_SPRITE_TABLE = {

    Sprite{SpriteID::EXPLOSION, TextureID::EFFECT_CANVAS, Rectangle{80, 48, 8 , 8}},
    Sprite{SpriteID::SHIELD_EFFECT, TextureID::EFFECT_CANVAS, Rectangle{88, 32, 16, 16}},

};


