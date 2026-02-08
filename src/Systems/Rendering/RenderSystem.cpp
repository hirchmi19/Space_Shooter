//
// Created by Michael Hirsch on 06.02.26.
//

#include "RenderSystem.h"

#include "raylib.h"
#include "../../GameWorld.h"
#include "../../Constants/GameConstants.h"

RenderSystem::RenderSystem() : IGameSystem(GameSystemID::RENDERER_SYSTEM) {}

void RenderSystem::Run(GameWorld& world) {

    RenderBackground(world);
}


void RenderSystem::RenderBackground(const GameWorld& world) const {

    world.RenderBackground();
}

void RenderSystem::RenderEnemies(const GameWorld &world) const {

}


void RenderSystem::RenderPlayer(const GameWorld &world) const {


}
