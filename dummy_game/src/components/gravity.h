#pragma once

#include <core/math.h>
#include "velocity.h"

struct gravity {
  bool previouslyOnFloor = false;
  bool onFloor = false;
  float entityGravity = 0.02f;
  float maxFallSpeed = 0.8f;

  void update(v2f *position, velocity *velocity, double dt) {
    previouslyOnFloor = onFloor;
    if (!onFloor) {
      velocity->v.y += (entityGravity * dt) / 10;
    }

    if (velocity->v.y > maxFallSpeed) {
      velocity->v.y = maxFallSpeed;
    }
  }
};
