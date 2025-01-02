#pragma once

#include <vector>

#define MAX_DELTA_TIME 7

enum class GAME_OBJECT {
  PLAYER,
  DOOR,
  NPC,
  ENEMY,
  SKELETON,
};

enum class OBJECT_TAG {
  SOLID,
  PLAYER,
};

struct GameState {
  std::vector<bool> visited;
};
