#include "renderer.h"
#include "logger.h"

Renderer::Renderer(SDL_Renderer* renderer) {
  _renderer = renderer;
}

void Renderer::renderSprite(Sprite* sprite, v2i position, bool useOffset) {
  SDL_Rect sr = sprite->textureRect.to_sdl_rect();
  int x = round(position.x);
  int y = round(position.y);
  SDL_Rect dr = { x, y, sr.w, sr.h };

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

void Renderer::renderRectTexture(Rect rectangle, SDL_Texture* texture, int tileWidth) {
  int framePadding = (tileWidth / 2);

  drawSide(SIDE::TOP, tileWidth, rectangle, texture);
  drawSide(SIDE::LEFT, tileWidth, rectangle, texture);
  drawSide(SIDE::BOTTOM, tileWidth, rectangle, texture);
  drawSide(SIDE::RIGHT, tileWidth, rectangle, texture);

  drawCorner(tileWidth, texture, rectangle.x, rectangle.y, SDL_FLIP_NONE);
  drawCorner(tileWidth, texture, rectangle.x + rectangle.w - tileWidth, rectangle.y, SDL_FLIP_HORIZONTAL);
  drawCorner(tileWidth, texture, rectangle.x,  rectangle.y + rectangle.h - tileWidth, SDL_FLIP_VERTICAL);
  drawCorner(tileWidth, texture, rectangle.x + rectangle.w - tileWidth, rectangle.y + rectangle.h - tileWidth, (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));

  SDL_Rect sr = { framePadding, framePadding, framePadding, framePadding };
  SDL_Rect dr = {
    rectangle.x + framePadding,
    rectangle.y + framePadding,
    rectangle.w - tileWidth,
    rectangle.h - tileWidth,
  };
  SDL_RenderCopyEx(_renderer, texture, &sr, &dr, 0, 0, SDL_FLIP_NONE);
}

void Renderer::drawSide(SIDE side, int tileWidth, Rect rect, SDL_Texture* texture) {
  SDL_Rect sr;
  SDL_Rect dr;
  int framePadding = (tileWidth / 2);

  if (side == SIDE::TOP) {
    dr = {
      rect.x + framePadding,
      rect.y,
      rect.w - tileWidth,
      tileWidth,
    };

    sr = { framePadding, 0, 1, tileWidth };
  }
  else if (side == SIDE::BOTTOM) {
    dr = {
      rect.x + framePadding,
      rect.y + rect.h - tileWidth,
      rect.w - tileWidth,
      tileWidth,
    };

    sr = { framePadding, tileWidth, 1, tileWidth };
  }
  else if (side == SIDE::LEFT) {
    dr = {
      rect.x,
      rect.y + framePadding,
      tileWidth,
      rect.h - tileWidth,
    };

    sr = { 0, framePadding, tileWidth, 1 };
  }
  else if (side == SIDE::RIGHT) {
    dr = {
      rect.x + rect.w - tileWidth,
      rect.y + framePadding,
      tileWidth,
      rect.h - tileWidth,
    };

    sr = { tileWidth, framePadding, tileWidth, 1 };
  }

  SDL_RenderCopyEx(_renderer, texture, &sr, &dr, 0, 0, SDL_FLIP_NONE);
}

void Renderer::drawCorner(int tileWidth, SDL_Texture* texture, const int x, const int y, const SDL_RendererFlip flip) {
  const int sr_x = flip & SDL_FLIP_HORIZONTAL ? tileWidth : 0;
  const int sr_y = flip & SDL_FLIP_VERTICAL & SDL_FLIP_VERTICAL ? tileWidth : 0;

  SDL_Rect sdl_sr = {
    sr_x,
    sr_y,
    tileWidth,
    tileWidth,
  };

  SDL_Rect sdl_dr = {
    x,
    y,
    tileWidth,
    tileWidth,
  };

  SDL_RenderCopyEx(_renderer, texture, &sdl_sr, &sdl_dr, 0, 0, SDL_FLIP_NONE);
}

void Renderer::loadSprite(const char* path, Sprite* sprite) {
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL) {
      LOG_ERROR("Image load error: Path(%s) - Error(%s)", path, IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

    if (texture == NULL) {
      LOG_ERROR("Failed to create texture");
    }

    if (surface == NULL || surface == NULL) {
      LOG_ERROR("Create texture error: %s", SDL_GetError());
    }

    Rect textureRect = { 0, 0, surface->w, surface->h };

    SDL_FreeSurface(surface); 

    sprite->texture = texture;
    sprite->textureRect = textureRect;
}
