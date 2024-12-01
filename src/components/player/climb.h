#pragma once

#include <cstdint>
#include <cstdlib>
#include "../../engine/timer.h"

namespace obj {
  struct Player;
};

struct ClimbController {
  obj::Player* player;

  /// How close we need to be to the ladder to climb it.
  const int LADDER_X_DEADZONE = 5;
  /// How high the initial jump should be
  const float JUMP_POWER = 0.4f;
  /// Jump buffer window in MS
  const uint32_t JUMP_BUFFER_WINDOW = 100;

  void init(obj::Player* player) { this->player = player; }
  bool update(double dt);
};
