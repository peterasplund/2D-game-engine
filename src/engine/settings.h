#pragma once

#include <cstdlib>

struct GameSettings {
  u_int16_t maxFrameRate;
  bool vsync;
};

GameSettings& gameSettings();
