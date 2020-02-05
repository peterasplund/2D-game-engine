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
  float jumpPower = 7.5f;
  float backDashSpeed = 1.5f;
  bool isBackDashing = false;
  float runSpeed = 3.0f;
  std::string direction = "right";
  State state = State::S_IDLE;
  // Timer timer;
};
