//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once
#include "raylib.h"

struct CombatComponent {

    Rectangle hitbox{};
    int score;
    float damage = 1;

    explicit CombatComponent(const float hp = 0, const Rectangle hitbox = {}, const int score = 0)
        : hitbox(hitbox), score(score), hp(hp) {}

    
    void TakeDamage(const float& dmg){

        if (dmg >= hp) hp = 0;
        else hp -= dmg;
    }
    
    bool IsAlive() const { return hp > 0; }
    void Kill() { hp = 0; }
    void Revive() { hp = 1; }


    private:

    float hp = 0;
};
