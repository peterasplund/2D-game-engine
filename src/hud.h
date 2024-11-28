#pragma once

#include "engine/assetManager.h"
#include "engine/math.h"
#include "engine/window.h"

class Hud {
protected:
  SDL_Texture *_barTexture;
  SDL_Texture *_barTexture2;
  v2i _size;

public:
  Hud() {
    _barTexture =
        AssetManager::Instance()->getTexture("assets/sprites/bar.png");
    _barTexture2 =
        AssetManager::Instance()->getTexture("assets/sprites/bar2.png");
    _size = {154, 9};
  }

  ~Hud() {}

  virtual void draw(SDL_Renderer *renderer) {
    SDL_Rect bgRectSrc = {0, 0, _size.x, _size.y};
    SDL_Rect bgRectDist = {4, 4, _size.x, _size.y};
    SDL_RenderCopy(renderer, _barTexture, &bgRectSrc, &bgRectDist);

    SDL_Rect bgRectDist2 = {4, 4 + 10, _size.x, _size.y};
    SDL_RenderCopy(renderer, _barTexture2, &bgRectSrc, &bgRectDist2);
  }
};
