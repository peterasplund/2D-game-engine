#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "math.h"
#include "renderable.h"
#include "camera.h"

class Renderer
{
  public:
    Renderer() {
    }

    ~Renderer() {
    }

    void draw(SDL_Renderer* renderer, Camera* camera, std::vector<Renderable*> renderables) {
      for (int i = 0; i < renderables.size(); i ++) {

      }
    }

  private:
};
