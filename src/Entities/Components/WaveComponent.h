//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once

#include "raylib.h"
#include "WaveState.h"


struct WaveComponent {

  Vector2 formationPosition;
  Vector2 worldPosition;
  int diveGroup;
  WaveState state = WaveState::OUT_FORMATION;

  std::array<Vector2, 4> controlPoints;
  float t = 0.0f;
  float speed = 0.6f;

};
