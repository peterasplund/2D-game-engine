
#pragma once
#include "SDL.h"

struct collidable {
  SDL_Rect rect = { 0, 0, 32, 32 };
  SDL_Rect prevRect;
};

// TODO: implement debug draw rect
