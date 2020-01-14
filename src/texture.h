#pragma once
#include "SDL.h"
#include "window.h"

class Texture
{
protected:
  SDL_Renderer* _renderer;
  SDL_Texture* _texture;
public:
  Texture(SDL_Renderer* renderer, std::string path) {
    _renderer = renderer;
  }

  ~Texture() {
  }

  virtual void render() {

  }
};
