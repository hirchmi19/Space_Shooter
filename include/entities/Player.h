//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once

#include "raylib.h"
#include <vector>
#include <cstddef>
#include "Entity.h"
#include "../components/CombatComponent.h"
#include "../components/Movement1D.h"
#include "../constants/RenderConstants.h"
#include "constants/MovementConstants.h"
#include "projectiles/ProjectileType.h"


class Player : public Entity {

    public:

    Player() = default;

    Player(
        const Vector2& position,
        const Vector2& size,
        const std::vector<const Sprite*>& sprites,
        const size_t& timer, const size_t& dashTimer, const size_t& projectileTimer)
        :
        Entity{position, {sprites, size},
            Rectangle{position.x, position.y,
            size.x * RenderConstants::PLAYER_SCALING,
            size.y * RenderConstants::PLAYER_SCALING}},
        movement{0, MovementConstants::BASE_SPEED},
        combat{1},
        cooldownTimer(timer), dashTimer(dashTimer), projectileTimer(projectileTimer) {}

    ~Player() = default;

    void SetPosition(const Vector2& pos);
    void SetProjectileType(const ProjectileType& pType);

    const Vector2& GetPosition() const { return position; }
    const Vector2& GetSize() const  { return render.size; }
    int GetDir() const { return movement.direction; }
    float GetSpeed() const { return movement.speed; }
    const Rectangle& GetHitBox() const { return hitbox; }

    void Revive() {isAlive = true; combat.Revive();}
    void Kill() {isAlive = false;}
    bool IsAlive() const { return isAlive; }
    void EnterFlowState() { inFlowState = true; }
    void ActivateShield() { shieldActive = true; }
    void DeactivateShield() { shieldActive = false; }
    void LeaveFlowState() { inFlowState = false; }
    bool IsInFlowState() const { return inFlowState; }
    bool IsShieldActive() const { return shieldActive; }

    void Run();

    private:

    Movement1D movement;
    CombatComponent combat;
    ProjectileType pType = ProjectileType::BASE_PLAYER;

    size_t cooldownTimer{};
    size_t dashTimer{};
    size_t projectileTimer{};

    bool inFlowState = false;
    bool shieldActive = false;

};
