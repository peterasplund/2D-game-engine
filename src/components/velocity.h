#pragma once

#include "../engine/math.h"

struct velocity {
  v2f v;
  float friction = 0.02f;
  float speed = 7.0f;
};
