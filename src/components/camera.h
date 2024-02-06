#pragma once

#include "../stdafx.h"
#include "../globals.h"
#include "position.h"
#include "../math.h"

struct camera {
  v2 viewport = { WINDOW_WIDTH / WINDOW_ZOOM, WINDOW_HEIGHT / WINDOW_ZOOM }; // viewport size. In this case same as window size. Make this less tall later when implementing the HUD.
  SDL_Rect bounds = { 0, 0, 2000, 352 }; // clamping bounds. Limit camera position within
  v2 pos = { 0, 0 }; // viewport size. In this case same as window size. Make this less tall when implementing the HUD.
  SDL_Rect* following; // entity to follow, eg. the player
  float zoom = 1.0f; // camera zoom

  void update() {
    float px, py, pw, ph;

    pw = following->w;
    ph = following->h;
    px = following->x;
    py = following->y;

    pos.x = px + (pw / 2) - (viewport.x / 2);
    pos.y = py + (ph / 2) - (viewport.y / 2);

    // clamp within bounds
    if (pos.x < bounds.x) { pos.x = bounds.x; }
    if (pos.x + viewport.x > bounds.w) { pos.x = bounds.w - viewport.x; }
    if (pos.y < bounds.y) { pos.y = bounds.y; }
    if (pos.y + viewport.y > bounds.h) { pos.y = bounds.h - viewport.y; }
  }

  void setBounds(v2i bounds) {
    this->bounds.w = bounds.x;
    this->bounds.h = bounds.y;
  }

  void follow(SDL_Rect* subject) {
    following = subject;
  }

  SDL_Rect getRect() {
    return {
      (int)pos.x,
      (int)pos.y,
      (int)viewport.x,
      (int)viewport.y
    };
  }
};
