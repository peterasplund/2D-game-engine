#pragma once
#include "window.h"
#include "renderer.h"

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
  virtual void update(float dt) {}
  virtual void draw(Renderer* renderer) {}
};
