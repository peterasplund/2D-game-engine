#pragma once

#include "SDL.h"
#include "math.h"
#include <memory>

struct Sprite {
  SDL_Texture* texture;
  SDL_Rect textureRect = { 0, 0, 32, 32 };
  SDL_RendererFlip textureFlip = SDL_FLIP_NONE;
  SDL_Point spriteOffset = { 0, 0 };
};

// @TODO: make offset optional in an ergonomic way
class Renderer {
public:
  Renderer(SDL_Renderer* renderer);
  void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b,  a);
  }
  void renderSprite(Sprite* sprite, v2i position, bool useOffset = true);
  void renderTexture(SDL_Texture* texture, Rect* sr, Rect* dr, SDL_RendererFlip textureFlip, bool useOffset = true);
  void renderRect(Rect* rectangle, bool useOffset = true);
  void renderRectFilled(Rect* rectangle, bool useOffset = true);
  void clearScreen() {
    SDL_SetRenderDrawColor(_renderer, 20, 20, 20, 255);
    SDL_RenderClear(_renderer);
  }

  void present() {
    SDL_RenderPresent(_renderer);
  }

  void setOffsetPtr(v2f* offset) {
    _offset = offset;
  }

private:
  SDL_Renderer* _renderer;
  /// Used for the camera
  v2f* _offset;

  SDL_Rect getOffsetRect(const Rect* rectangle) {
    SDL_Rect r = rectangle->to_sdl_rect();

    if (_offset == nullptr) {
      return r;
    }

    r.x -= _offset->x;
    r.y -= _offset->y;

    return r;
  }

  SDL_Rect getOffsetRect(const SDL_Rect* rectangle) {
    SDL_Rect r = *rectangle;

    if (_offset == nullptr) {
      return r;
    }

    r.x -= _offset->x;
    r.y -= _offset->y;

    return r;
  }
};
