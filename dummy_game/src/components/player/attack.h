#pragma once

#include <cstdint>
#include <cstdlib>
#include <core/timer.h>
#include <core/renderer.h>

namespace obj {
  struct Player;
};

struct AttackController {
  obj::Player* player;
  bool firstAttack = true; 
  Timer attackTimer;
  RectF swordHitBox;

  float attackDelay = 280.0f;

  void init(obj::Player* player) { this->player = player; }
  void update(double dt);
  void onInputPressed(int button);
  void attack();

  void draw(Renderer *renderer);
};
