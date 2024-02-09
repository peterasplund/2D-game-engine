#pragma once
#include "../stdafx.h"
#include "../math.h"

class Renderable {
  public:
    SDL_Texture* texture;
    SDL_Rect textureRect = { 0, 0, 32, 32 };
    SDL_RendererFlip textureFlip = SDL_FLIP_NONE;

    void render(SDL_Renderer* renderer, v2 position) {
      SDL_Rect sr = textureRect;
      SDL_Rect dr = { (int)round(position.x), (int)round(position.y), sr.w, sr.h };

      SDL_RenderCopyEx(renderer, texture, &sr, &dr, 0, 0, textureFlip);
    }
};

