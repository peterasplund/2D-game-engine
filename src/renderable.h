#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <string>

class Renderable
{
private:
  SDL_Renderer* renderer;
  SDL_Surface* surface;
  SDL_Texture* texture;
  SDL_Rect rectangle;
public:
  Renderable(SDL_Renderer* renderer, std::string path) {
    SDL_Surface* surface = IMG_Load(path.c_str());

    if (!surface) {
      printf("error creating surface\n");
      return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
      printf("error creating texture: %s\n", SDL_GetError());
      return;
    }

    SDL_Rect rectangle = {40, 40, 32, 32};

    this->renderer  = renderer;
    this->surface   = surface;
    this->texture   = texture;
    this->rectangle = rectangle;
  }

  ~Renderable();

  void move(int x, int y) {
    this->rectangle.x = x;
    this->rectangle.y = y;
  }

  void render() {
    SDL_RenderCopy(this->renderer, this->texture, NULL, &this->rectangle);
  }
};
