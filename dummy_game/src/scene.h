#pragma once

#include <core/renderer.h>
#include <core/defines.h>

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
  virtual void update(f64 dt) {}
  virtual void draw(Renderer* renderer) {}
};
