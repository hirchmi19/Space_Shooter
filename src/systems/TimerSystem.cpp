//
// Created by Michael Hirsch on 01.03.26.
//

#include <ranges>
#include "systems/timer/TimerSystem.h"

#include <cassert>

#include "constants/GameConstants.h"

TimerSystem::TimerSystem() : IGameSystem(GameSystemID::TIMER_SYSTEM, "TIMER_SYSTEM") {

    timers.reserve(500);
}
void TimerSystem::Run() {

    for (size_t index = 0; index < timers.size(); index++) {

        if (!timers[index].isRunning) continue;

        const float delta = 1.0f / GameConstants::UPS;
        timers[index].elapsedTime += delta;
        timers[index].timeLeft -= delta;

        if (timers[index].elapsedTime >= timers[index].duration) {

            timers[index].isRunning = false;
            timers[index].elapsedTime = 0;
            timers[index].timeLeft = timers[index].duration;
            if (!timers[index].disposable) continue;

            const auto it = std::ranges::lower_bound(deadTimers, index);
            deadTimers.insert(it, index);
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

    assert(timers.size() < timers.capacity() && "TIMER CAPACITY REACHED!");

    timers.emplace_back(duration, 0, duration, disposable);
    timers.back().isRunning = duration > 0;
    if (!disposable) permanentTimerCount++;
    return timers.size() - 1;
}

bool TimerSystem::IsRunning(const size_t &index) {

    return timers[index].isRunning;
}

void TimerSystem::KillTimers() {

    assert(timers.size() - permanentTimerCount <= 100 && "TOO MANY DISPOSABLE TIMERS!");

    timers.resize(permanentTimerCount);
    deadTimers.clear();
}
