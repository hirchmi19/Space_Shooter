//
// Created by Michael Hirsch on 22.02.26.
//

#include <cstdint>
enum class GameState : uint32_t {

    NONE,
    BEGIN_WAVE,
    IN_GAME,
    END_WAVE,
    GAME_OVER,
    COUNT

};