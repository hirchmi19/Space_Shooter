//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once

#include "raylib.h"
#include <vector>
#include "CombatData.h"
#include "Movement1D.h"
#include "RenderData.h"
#include "../Systems/Assets/Sprite.h"

class GameWorld;

class Player {

    public:
    Player(const Vector2& position, const Vector2& size, const std::vector<const Sprite*>& sprites)
     : movement{ position, 0.0f }, render{ sprites, size }, combat{3, Rectangle {position.x, position.y, size.x, size.y} } {}

    ~Player() = default;

    void SetPosition( const Vector2& newPosition) {

        movement.position = newPosition;
        combat.hitbox.x = newPosition.x;
        combat.hitbox.y = newPosition.y;
    }

    const Vector2& GetPosition() const { return movement.position; }
    const Vector2& GetSize() const  { return render.size; }
    float GetSpeed() const { return movement.speed; }
    bool IsAlive() const { return combat.IsAlive(); }

    void HandleInput(GameWorld& world);

    private:
    Movement1D movement;
    RenderData render;
    CombatData combat;
};
