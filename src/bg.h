#pragma once
#include "engine/assetManager.h"
#include "engine/math.h"

class Bg {
protected:
  SDL_Texture *_texture;
  v2i _size;

public:
  Bg(std::string path, v2i size) {
    _texture = AssetManager::Instance()->getTexture(path);
    _size = size;
  }

  ~Bg() {}

  virtual void draw(SDL_Renderer *renderer, float offset = 0.0f) {
    float localOffset = (int)offset % _size.x;
    SDL_Rect bgRectSrc = {0, 0, _size.x, _size.y};
    SDL_Rect bgRectDist = {(int)localOffset, 0, _size.x, _size.y};
    SDL_RenderCopy(renderer, _texture, &bgRectSrc, &bgRectDist);

    if (offset != 0.0f) {
      SDL_Rect bgRectDist2 = {_size.x + (int)localOffset, 0, _size.x, _size.y};
      SDL_RenderCopy(renderer, _texture, &bgRectSrc, &bgRectDist2);
    }
  }
};
