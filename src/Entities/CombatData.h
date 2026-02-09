//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

struct CombatData {

    uint32_t health;
    Rectangle hitbox;
    bool isAlive = true;

    bool IsAlive() const { return isAlive; }
    void Kill() { isAlive = false; }

};