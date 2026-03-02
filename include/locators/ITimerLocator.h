//
// Created by Michael Hirsch on 01.03.26.
//

#pragma once
#include <cstddef>

struct ITimerLocator {

  virtual ~ITimerLocator() = default;

  virtual void Start(const float &duration, const size_t& index) = 0;
  virtual const size_t CreateTimer(const float &duration, bool disposable) = 0;
  virtual bool IsRunning(const size_t& index) = 0;
  virtual float GetTimeLeft (const size_t& index) = 0;
  virtual void KillTimers() = 0;

};