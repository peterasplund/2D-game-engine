#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <map>
#include "../inputHandler.h"
#include "../timer.h"

enum State {
  S_IDLE,
  S_RUN,
  S_JUMP,
  S_ATTACK,
};

struct characterController {
  float _jumpPower = 3.0f;
  float backDashSpeed = 1.5f;
  bool isBackDashing = false;
  float runSpeed = 1.1f;
  std::string direction = "right";
  State _state = State::S_IDLE;
  // Timer _timer;
};
