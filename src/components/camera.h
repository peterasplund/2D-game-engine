#pragma once

#include "SDL.h"
#include "position.h"
#include "../math.h"

struct camera {
  v2 viewport = { 512, 352 }; // viewport size. In this case same as window size. Make this less tall when implementing the HUD.
  v2 pos = { 0, 0 }; // viewport size. In this case same as window size. Make this less tall when implementing the HUD.
  SDL_Rect bounds = { 0, 0, 2000, 2000 }; // clamping bounds. Limit camera position within
  position* following; // entity to follow, eg. the player
  float zoom = 1.0f; // camera zoom
};

namespace Camera {
  void follow(camera *camera, position* subject) {
    camera->following = subject;
  }

    SDL_Rect getRect(camera *camera) {
      return {
        (int)camera->pos.x,
        (int)camera->pos.y,
        (int)camera->viewport.x,
        (int)camera->viewport.y
      };
    }
}
