#pragma once
#include "window.h"

class Scene
{
protected:
  SDL_Renderer* _renderer;
public:
  Scene(SDL_Renderer* renderer) {
    _renderer = renderer;
  }

  ~Scene() {}

  virtual void init() {}
  virtual void update(float dt) {}
  virtual void draw(SDL_Renderer* renderer) {}
};
