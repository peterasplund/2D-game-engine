#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "math.h"
#include "object.h"
#include "animation.h"
#include <map>
#include <string>

class Camera
{
  public:
    Camera(v2 size, SDL_Rect bounds) {
      this->size = size;
      this->bounds = bounds;

      position = { 0.0f, 0.0f };
    }

    ~Camera() {
    }

    void setBounds(SDL_Rect bounds) {
      this->bounds = bounds;
    }

    void move(int x, int y) {
      position.x = x;
      position.y = y;
    }

    void follow(Object* other) {
      following = other;
    }

    SDL_Rect getRect() {
      return { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    }

    void update(float dt) {
      if (following != nullptr) {
        position.x = following->getPosition().x + (following->getSize().x / 2) - (size.x / 2);
        position.y = following->getPosition().y + (following->getSize().y / 2) - (size.y / 2);

        // clamp
        if (position.x < bounds.x) { position.x = bounds.x; }
        if (position.x > bounds.w) { position.x = bounds.w; }
        if (position.y < bounds.y) { position.y = bounds.y; }
        if (position.y > bounds.h) { position.x = bounds.h; }
      }
    }

  private:
    v2 position;
    SDL_Rect bounds;
    v2 size;
    Object* following;
};
