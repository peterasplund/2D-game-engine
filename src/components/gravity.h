#pragma once

#include "math.h"
#include "velocity.h"

struct gravity {
  bool onFloor = false;
  float entityGravity = 0.18f;
  float maxFallSpeed = 6.5f;

  void update(v2* position, velocity* velocity, float dt) {
    if (velocity->y < 0.0f) {
      onFloor = false;
    }

    if (!onFloor) {
      velocity->y += (entityGravity * dt) / 10;
    }

    if (velocity->y > maxFallSpeed) {
      velocity->y = maxFallSpeed;
    }
  }
};
