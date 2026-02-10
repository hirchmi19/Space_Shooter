//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once

#include "raylib.h"
#include <vector>
#include "Components/CombatComponent.h"
#include "Components/Movement1D.h"
#include "Components/RenderComponent.h"
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

    void SetPosition(const Vector2& pos) {

        movement.position = pos;
        combat.hitbox.x = pos.x;
        combat.hitbox.y = pos.y;
    };

    const Vector2& GetPosition() const { return movement.position; }
    const Vector2& GetSize() const  { return render.size; }
    int32_t GetSpeed() const { return movement.speed; }
    const Rectangle& GetHitBox() const { return combat.hitbox; }
    bool IsAlive() const { return combat.IsAlive(); }

    void HandleInput(GameWorld& world);

    private:
    Movement1D movement;
    RenderComponent render;
    CombatComponent combat;
};
