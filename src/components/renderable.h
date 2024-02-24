#pragma once
#include "../math.h"

class Renderable {
  public:
    SDL_Texture* texture;
    SDL_Rect textureRect = { 0, 0, 32, 32 };
    SDL_RendererFlip textureFlip = SDL_FLIP_NONE;

    void render(SDL_Renderer* renderer, v2f position) {
      SDL_Rect sr = textureRect;
      int x = round(position.x);
      int y = round(position.y);
      SDL_Rect dr = { x, y, sr.w, sr.h };

      //SDL_RenderCopyEx(renderer, texture, &sr, &dr, 0, 0, textureFlip);
    }
};

