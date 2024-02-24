#pragma once

#include "math.h"
#include "velocity.h"

struct gravity {
  bool onFloor = false;
  float entityGravity = 0.18f;
  float maxFallSpeed = 6.5f;

  void update(v2f* position, velocity* velocity, float dt) {
    if (velocity->v.y < 0.0f) {
      onFloor = false;
    }

    if (!onFloor) {
      velocity->v.y += (entityGravity * dt) / 10;
    }

    if (velocity->v.y > maxFallSpeed) {
      velocity->v.y = maxFallSpeed;
    }
  }
};
