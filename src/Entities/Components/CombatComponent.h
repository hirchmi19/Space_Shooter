//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

struct CombatComponent {

    Rectangle hitbox{};
    uint32_t score;
    float damage = 1;

    explicit CombatComponent(const float hp = 0, const Rectangle hitbox = {}, const uint32_t score = 0)
        : hitbox(hitbox), health(hp), score(score) {}

    bool IsAlive() const { return health > 0; }
    void Kill() { health = 0; }

    void TakeDamage(const float dmg) {

        if (dmg >= health) {health = 0;}
        else {health -= dmg;}
    }

    void Revive() { health = 1; }

    private:
    float health = 0;
};
