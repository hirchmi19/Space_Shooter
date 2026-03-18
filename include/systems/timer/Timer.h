//
// Created by Michael Hirsch on 01.03.26.
//

#pragma once

struct Timer {

    float duration = 0.f;
    float elapsedTime = 0.f;
    float timeLeft;
    bool disposable;
    bool isRunning = false;
    bool isPaused = false;

};