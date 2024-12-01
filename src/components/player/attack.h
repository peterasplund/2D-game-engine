#pragma once

#include <cstdint>
#include <cstdlib>
#include "../../engine/timer.h"
#include "../../engine/renderer.h"

namespace obj {
  struct Player;
};

struct AttackController {
  obj::Player* player;

  Timer attackTimer;
  float attackDelay = 280.0f;
  RectF swordHitBox;

  void init(obj::Player* player) { this->player = player; }
  void update(double dt);
  void onInputPressed(int button);
  void attack();

  void draw(Renderer *renderer);
};
