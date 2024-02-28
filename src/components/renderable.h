#pragma once
#include "../math.h"
#include "../renderer.h"

class Renderable {
  public:
    SDL_Texture* texture;
    Rect textureRect = { 0, 0, 32, 32 };
    SDL_RendererFlip textureFlip = SDL_FLIP_NONE;
    SDL_Point spriteOffset = { 0, 0 };

    void render(Renderer* renderer, v2f position) {
      Rect sr = textureRect;
      int x = round(position.x);
      int y = round(position.y);
      Rect dr = { x, y, sr.w, sr.h };

      if (textureFlip == SDL_FLIP_HORIZONTAL) {
        dr.x -= spriteOffset.x;
      }

      renderer->renderTexture(texture, &sr, &dr, textureFlip);
    }
};

