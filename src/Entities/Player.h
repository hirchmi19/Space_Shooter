//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once

#include "raylib.h"
#include <vector>
#include "CombatData.h"
#include "Movement1D.h"
#include "RenderData.h"
#include "../Constants/RenderConstants.h"
#include "../Systems/Assets/Sprite.h"

class GameWorld;

class Player {

    public:
    Player(const Vector2& position, const Vector2& size, const std::vector<const Sprite*>& sprites)
     : movement{ position, 0 }, render{ sprites, size }, combat{3,
         Rectangle {position.x, position.y, size.x * RenderConstants::PLAYER_SCALING, size.y * RenderConstants::PLAYER_SCALING} }
    {}

    ~Player() = default;

    void CalcNewPosition(const int32_t& speed);

    const Vector2& GetPosition() const { return movement.position; }
    const Vector2& GetSize() const  { return render.size; }
    const int32_t& GetSpeed() const { return movement.speed; }
    const Rectangle& GetHitBox() const { return combat.hitbox; }
    bool IsAlive() const { return combat.IsAlive(); }

    void HandleInput(GameWorld& world);

    private:
    Movement1D movement;
    RenderData render;
    CombatData combat;
};
