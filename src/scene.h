#pragma once

#include "engine/renderer.h"

class Scene
{
protected:
  Renderer* _renderer;
public:
  Scene(Renderer* renderer) {
    _renderer = renderer;
  }

  virtual ~Scene() {}

  virtual void init() {}
  virtual void update(double dt) {}
  virtual void draw(Renderer* renderer) {}
};
