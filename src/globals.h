#pragma once

#include <vector>

#define WINDOW_WIDTH   480*4
#define WINDOW_HEIGHT  272*4
#define WINDOW_ZOOM    4
#define WINDOW_FPS     60
#define MAX_DELTA_TIME 7

enum class GAME_OBJECT {
  PLAYER,
  DOOR,
  NPC,
};

enum class OBJECT_TAG {
  SOLID,
  PLAYER,
};

struct GameState {
  std::vector<bool> visited;
};
