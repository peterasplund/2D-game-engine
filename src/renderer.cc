#include "renderer.h"

Renderer::Renderer(SDL_Renderer* renderer) {
  _renderer = renderer;
}

void Renderer::renderSprite(Sprite* sprite, v2i position, bool useOffset) {
  SDL_Rect sr = sprite->textureRect;
  int x = round(position.x);
  int y = round(position.y);
  SDL_Rect dr = { x, y, sr.w, sr.h };

  SDL_Rect r = useOffset ? getOffsetRect(&dr) : dr;

  if (sprite->textureFlip == SDL_FLIP_HORIZONTAL) {
    dr.x -= sprite->spriteOffset.x;
  }

  SDL_RenderCopyEx(_renderer, sprite->texture, &sr, &dr, 0, 0, sprite->textureFlip);
}

void Renderer::renderTexture(SDL_Texture* texture, Rect* sr, Rect* dr, SDL_RendererFlip textureFlip, bool useOffset) {
  SDL_Rect sdl_sr = sr->to_sdl_rect();
  SDL_Rect sdl_dr = useOffset ? getOffsetRect(dr) : dr->to_sdl_rect();

  SDL_RenderCopyEx(_renderer, texture, &sdl_sr, &sdl_dr, 0, 0, textureFlip);
}

void Renderer::renderRect(Rect* rectangle, bool useOffset) {
  SDL_Rect r = useOffset ? getOffsetRect(rectangle) : rectangle->to_sdl_rect();
  SDL_RenderDrawRect(_renderer, &r);
}

void Renderer::renderRectFilled(Rect* rectangle, bool useOffset) {
  SDL_Rect r = useOffset ? getOffsetRect(rectangle) : rectangle->to_sdl_rect();
  SDL_RenderFillRect(_renderer, &r);
}
