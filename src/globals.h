#pragma once

const int WINDOW_WIDTH     = 1920;
const int WINDOW_HEIGHT    = 1080;
const int WINDOW_ZOOM      = 4;
const int WINDOW_FPS       = 60;
const float MAX_DELTA_TIME = 7;

enum class GAME_OBJECT {
  PLAYER,
  DOOR,
  NPC,
};

enum class OBJECT_TAG {
  SOLID,
  PLAYER,
};