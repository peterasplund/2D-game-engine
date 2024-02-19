#pragma once
#include "window.h"
#include "math.h"
#include "assetManager.h"

class Bg
{
protected:
  SDL_Texture* _texture;
  v2 _size;
public:
  Bg(std::string path, v2 size, SDL_Renderer* renderer) {
    _texture = AssetManager::Instance(renderer)->getTexture(path);
    _size = size;
  }

  ~Bg() {

  }

  virtual void draw(SDL_Renderer* renderer, float offset = 0.0f) {
    float localOffset = (int)offset % (int)_size.x;
    SDL_Rect bgRectSrc = { 0, 0, (int)_size.x, (int)_size.y };
    SDL_Rect bgRectDist = { (int)localOffset, 0, (int)_size.x, (int)_size.y };
    SDL_RenderCopy(renderer, _texture, &bgRectSrc, &bgRectDist);

    if (offset != 0.0f) {
      SDL_Rect bgRectDist2 = { (int)_size.x + (int)localOffset, 0, (int)_size.x, (int)_size.y };
      SDL_RenderCopy(renderer, _texture, &bgRectSrc, &bgRectDist2);
    }
  }
};
