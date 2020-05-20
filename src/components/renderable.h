#pragma once
#include "../stdafx.h"

struct renderable {
  SDL_Texture* texture;
  SDL_Rect textureRect = { 0, 0, 32, 32 };
  SDL_RendererFlip textureFlip = SDL_FLIP_NONE;
};
