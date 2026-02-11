//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once
#include "../../Systems/Timers/Timer.h"

struct CombatComponent {

    public:
    Rectangle hitbox{};

    explicit CombatComponent(const int32_t hp, const Rectangle hitbox)
        : hitbox(hitbox), health(hp) {}

    bool IsAlive() const { return health > 0; }
    void Kill() { health = 0; }

    void TakeDamage(const int damage) {

        if (damage >= health) {health = 0;}
        else {health -= damage;}
    }

    private:
    int32_t health = 0;
};
