//
// Created by Michael Hirsch on 01.03.26.
//

#pragma once
#include <vector>

#include "Timer.h"
#include "../../game/IGameSystem.h"
#include "../../locators/ITimerLocator.h"


class TimerSystem : public ITimerLocator, public IGameSystem{

    public:
    TimerSystem();
    ~TimerSystem() override = default;

    void Run() override;
    void Start(const float &duration, const size_t& index) override;
    const size_t CreateTimer(const float &duration, bool disposable) override;
    bool IsRunning(const size_t& index) override;
    float GetTimeLeft(const size_t& index) override {return timers[index].timeLeft; }
    void KillTimers() override;
    void ClearTimers() override { timers.clear(); }

    private:

    size_t permanentTimerCount = 0;
    std::vector<Timer> timers{};

};


