
#pragma once
#include "../stdafx.h"
#include "../math.h"

class collidable {
  public:
    SDL_Rect boundingBox;
    SDL_Rect rect;
    SDL_Rect prevRect;

    collidable() {
    }

    collidable(v2 position, SDL_Rect boundingBox) {
      this->boundingBox = boundingBox;

      update(position);
    }

    SDL_Rect addBoundingBox(v2 p) {
      return SDL_Rect {
        (int)p.x + boundingBox.x,
        (int)p.y + boundingBox.y,
        boundingBox.w,
        boundingBox.h
      };
    }

    void update(v2 position) {
      prevRect = rect;

      rect = {
        (int)position.x + boundingBox.x,
        (int)position.y + boundingBox.y,
        boundingBox.w,
        boundingBox.h
      };
    }
};