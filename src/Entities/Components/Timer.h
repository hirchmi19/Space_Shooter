//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once

struct Timer {

    double duration = 0.0;
    double elapsedTime = 0.0;

    void Start(const double newDuration) {
        duration = newDuration;
        elapsedTime = 0.0;
    }

    void Tick(const float deltaTime) {
        if (IsRunning())
            elapsedTime += deltaTime;
    }

    bool IsRunning() const {
        return elapsedTime < duration;
    }

    bool IsFinished() const {
        return elapsedTime >= duration;
    }
};
