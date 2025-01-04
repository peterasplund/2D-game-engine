#pragma once

#include <cstdint>
#include <cstdlib>
#include "../../engine/timer.h"

namespace obj {
  struct Player;
};

struct SlideController {
  obj::Player* player;

  /// How long to slide
  const float SLIDE_POWER = 0.45f;
  /// How fast to get to a stop
  const float SLIDE_DEACCELERATION = 0.0009f;
  float slideDelay = 450.0f;
  Timer slideTimer;

  void init(obj::Player* player) { this->player = player; }
  void update(double dt);
  void onInputPressed(int button);
  void slide();
};
