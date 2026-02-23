//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once

struct TimerComponent {

    void Start(const float newDuration) {

        duration = newDuration;
        elapsedTime = 0.0f;
    }

    void Tick(const float deltaTime) {

        if (IsRunning())elapsedTime += deltaTime;
    }

    bool IsRunning() const {return elapsedTime < duration;}

    bool IsFinished() const {return elapsedTime >= duration;}

    private:
    float duration = 0.0f;
    float elapsedTime = 0.0f;
};
