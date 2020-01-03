#pragma once

enum SubWeapon {
  None,
  Knife,
  Axe,
};

struct GameState {
  int life = 100;
  int score = 0;
  SubWeapon subWeapon = SubWeapon::None;
};

