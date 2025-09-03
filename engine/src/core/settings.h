#pragma once

#include <cstdlib>

struct GameSettings {
  u_int16_t maxFrameRate;
  bool vsync;
  int zoom = 4; // Set 4x zoom for the pixelated look
  int windowWidth = 480;
  int windowHeight = 272;
};

GameSettings& gameSettings();
