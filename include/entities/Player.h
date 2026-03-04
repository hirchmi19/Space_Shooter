//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once

#include "raylib.h"
#include <vector>

#include "../components/CombatComponent.h"
#include "../components/Movement1D.h"
#include "../components/RenderComponent.h"
#include "../constants/RenderConstants.h"


class Player {

    public:

    Player() = default;

    Player(const Vector2& position, const Vector2& size, const std::vector<const Sprite*>& sprites, const size_t& timer)
     : movement{ position, 0 },
    render{ sprites, size },
    combat{1,
         Rectangle {position.x, position.y,
             size.x * RenderConstants::PLAYER_SCALING,
             size.y * RenderConstants::PLAYER_SCALING} }
    , cooldownTimer(timer) {}

    ~Player() = default;

    void SetPosition(const Vector2& pos);
    const Vector2& GetPosition() const { return movement.position; }
    const Vector2& GetSize() const  { return render.size; }
    int GetSpeed() const { return movement.speed; }
    const Rectangle& GetHitBox() const { return combat.hitbox; }

    void Revive() {combat.Revive();}
    void Kill() {combat.Kill();}
    bool IsAlive() const { return combat.IsAlive(); }
    void EnterFlowState() { inFlowState = true; }
    void LeaveFlowState() { inFlowState = false; }
    bool IsInFlowState() const { return inFlowState; }

    void HandleInput();

    private:

    Movement1D movement;
    RenderComponent render;
    CombatComponent combat;
    size_t cooldownTimer{};
    bool inFlowState = false;
};
