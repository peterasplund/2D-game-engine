#pragma once

#include <cstdint>
#include <cstdlib>
#include "../../engine/timer.h"

namespace obj {
  struct Player;
};

struct JumpController {
  obj::Player* player;

  Timer jumpBufferTimer;
  bool jumpHold = false;
  bool justJumped = false;
  bool jumpBuffered = false;
  /// How high the initial jump should be
  const float JUMP_POWER = 0.4f;
  /// Jump buffer window in MS
  const uint32_t JUMP_BUFFER_WINDOW = 100;

  void init(obj::Player* player) { this->player = player; }

  void update(double dt);
  void onInputPressed(int button);
  void onInputReleased(int button);
  void jump();
  void performJump();
};
