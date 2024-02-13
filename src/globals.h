#pragma once

const int WINDOW_WIDTH     = 512 * 2;
const int WINDOW_HEIGHT    = 352 * 2;
const int WINDOW_ZOOM      = 2;
const int WINDOW_FPS       = 60;
const float MAX_DELTA_TIME = 7;

enum class GAME_OBJECT {
  PLAYER,
  DOOR,
};

enum class OBJECT_TAG {
  SOLID,
  PLAYER,
};