//
// Created by Michael Hirsch on 11.02.26.
//

#pragma once

#include "raylib.h"
#include "WaveState.h"


struct WaveComponent {

  Vector2 formationPosition;
  Vector2 worldPosition;
  Vector2 spawnPosition;

  int diveGroup;
  int slotIndex;
  WaveState state = WaveState::OUT_FORMATION;

  std::array<Vector2, 4> controlPoints;
  float t = 0.0f; // shows dive progress t = {0,1}; 0 at the beginning of the dive; 1 at the end
  float speed = 0.7f;

};
