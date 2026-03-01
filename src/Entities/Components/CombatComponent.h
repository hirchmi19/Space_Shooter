//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

struct CombatComponent {

    Rectangle hitbox{};
    uint32_t score;
    float damage = 1;

    explicit CombatComponent(const float hp = 0, const Rectangle hitbox = {}, const uint32_t score = 0)
        : hitbox(hitbox), score(score), hp(hp) {}

    bool IsAlive() const { return hp > 0; }
    void Kill() { hp = 0; }
    void Revive() { hp = 1; }


    private:

    float hp = 0;
};
