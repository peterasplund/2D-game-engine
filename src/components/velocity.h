#pragma once

#include "../engine/math.h"

struct velocity {
  float calcFriction(float v, float friction, double dt) {
    if (v < 0) {
      return std::min<float>(0.0f, v + (friction * dt));
    }
    else if (v > 0) {
      return std::max<float>(0.0f, v - (friction * dt));
    }

    return 0.0f;
  }

  v2f v;
  float friction = 0.02f;
  float speed = 7.0f;
};
