//
// Created by Michael Hirsch on 09.02.26.
//

#include "MovementSystem.h"

#include "../../Game/GameWorld.h"
#include "../../Constants/RenderConstants.h"

MovementSystem::MovementSystem() : IGameSystem(GameSystemID::MOVEMENT_SYSTEM){}

void MovementSystem::Run(GameWorld& world) {

    MovePlayer(world);

}

//--------------------------------------------------------------------------


void MovementSystem::MovePlayer(GameWorld& world) {

    Player& player = world.GetPlayer();
    const int32_t& playerSpeed = player.GetSpeed() * PLAYER_WORLD_SPEED;
    const Vector2& playerPosition = player.GetPosition();
    const uint32_t playerPosXRight = playerPosition.x + player.GetSize().x * RenderConstants::PLAYER_SCALING;

    if (playerPosXRight > GameConstants::SCREEN_WIDTH && playerSpeed > 0)return;
    if (playerPosition.x <= 0 && playerSpeed < 0) return;

    player.CalcNewPosition(playerSpeed);
}
