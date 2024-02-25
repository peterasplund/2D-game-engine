#pragma once

#include "math.h"
#include "velocity.h"

struct gravity {
  bool onFloor = false;
  float entityGravity = 0.02f;
  float maxFallSpeed = 0.8f;

  void update(v2f* position, velocity* velocity, float dt) {
    if (!onFloor) {
      velocity->v.y += (entityGravity * dt) / 10;
    }

    if (velocity->v.y > maxFallSpeed) {
      velocity->v.y = maxFallSpeed;
    }
  }
};
