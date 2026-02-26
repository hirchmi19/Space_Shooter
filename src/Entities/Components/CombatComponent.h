//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

struct CombatComponent {

    public:
    uint32_t score;
    Rectangle hitbox{};

    explicit CombatComponent(const int hp = 0, const Rectangle hitbox = {}, const uint32_t score = 0)
        : hitbox(hitbox), health(hp), score(score) {}

    bool IsAlive() const { return health > 0; }
    void Kill() { health = 0; }

    void TakeDamage(const int damage) {

        if (damage >= health) {health = 0;}
        else {health -= damage;}
    }

    void Revive() { health = 1; }

    private:
    uint32_t health = 0;
};
