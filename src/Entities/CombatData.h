//
// Created by Michael Hirsch on 08.02.26.
//

#pragma once

struct CombatData {

    int health;
    Rectangle hitbox;

    bool IsAlive() const { return health > 0; }

};