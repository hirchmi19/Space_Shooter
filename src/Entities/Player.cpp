//
// Created by Michael Hirsch on 03.02.26.
//

#include "raylib.h"
#include "Player.h"

#include "../Game/GameWorld.h"

void Player::SetPosition(const Vector2& pos) {

    movement.position = pos;
    combat.hitbox.x = pos.x;
    combat.hitbox.y = pos.y;
};



void Player::HandleInput(GameWorld& world) {

    movement.speed = 0;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.speed = -1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.speed = 1;

    if (IsKeyPressed(KEY_SPACE)) {

        if (!world.GetPlayer().CanShoot()) return;

        world.SpawnPlayerProjectile(movement.position);
        world.GetPlayer().GetTimer().Start(0.7);
    }

}
