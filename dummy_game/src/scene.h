#pragma once

#include <core/defines.h>
#include <core/renderer.h>

class Scene {
protected:
  Renderer *_renderer;

public:
  Scene(Renderer *renderer) { _renderer = renderer; }

  virtual ~Scene() {}

  virtual void init() {}
  virtual void update(f64 dt) {}
  virtual void draw(Renderer *renderer) {}
};
