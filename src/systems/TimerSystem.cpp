//
// Created by Michael Hirsch on 01.03.26.
//

#include <ranges>
#include "systems/timer/TimerSystem.h"
#include "constants/GameConstants.h"

TimerSystem::TimerSystem() : IGameSystem(GameSystemID::TIMER_SYSTEM, "TIMER_SYSTEM") {

    timers.reserve(50);
}
void TimerSystem::Run() {

    for (size_t index = 0; index < timers.size(); index++) {

        if (!timers[index].isRunning) continue;

        timers[index].elapsedTime += 1 / GameConstants::UPS;
        timers[index].timeLeft -= timers[index].elapsedTime;

        if (timers[index].elapsedTime >= timers[index].duration) {

            timers[index].isRunning = false;
            timers[index].elapsedTime = 0;
            timers[index].timeLeft = timers[index].duration;
            if (!timers[index].disposable) return;

            const auto it = std::ranges::lower_bound(deadTimers, index);
            deadTimers.insert(it, index); // insert timer, so dead timers is sorted

        }
    }
}

void TimerSystem::Start(const float &duration, const size_t &index) {

    timers[index].isRunning = true;
    timers[index].elapsedTime = 0;
    timers[index].duration = duration;
    timers[index].timeLeft = duration;
}

const size_t TimerSystem::CreateTimer(const float &duration, bool disposable) {

    timers.emplace_back(duration, 0, duration, disposable);
    timers.back().isRunning = true;
    return timers.size() - 1;


}

bool TimerSystem::IsRunning(const size_t &index) {

    return timers[index].isRunning;
}

void TimerSystem::KillTimers() {

    for (const auto& timer : std::ranges::reverse_view(deadTimers)) {

       timers.erase(timers.begin() + static_cast<long>(timer));

    }

    deadTimers.clear();
}
