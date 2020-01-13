#pragma once
#include <SDL2/SDL.h>
#include "object.h"

class PhysicsObject : public Object
{
protected:
  bool onFloor = false;

  float _gravity = 0.04f;
  float _maxFallSpeed = 3.5f;
  float friction = 0.12f;
public:

  PhysicsObject() : Object() {
  }

  virtual void onTopCollision(Object* other) {
  }

  virtual void onBottomCollision(Object* other) {
  }

  virtual void onLeftCollision(Object* other) {
  }

  virtual void onRightCollision(Object* other) {
  }

  void resolveCollisions(std::vector<Object> tiles) {
    onFloor = false;
    
    for (int j = 0; j < tiles.size(); j ++) {
      auto tile = tiles[j];

      if (!tile.getSolid()) {
        continue;
      }

      // collision
      if (Object::intersectsWith(&tile)) {
        SDL_Rect r1 = getHitBox();
        SDL_Rect r2 = tile.getHitBox();

        float r1t = r1.y;
        float r1b = r1.y + r1.h;
        float r1l = r1.x;
        float r1r = r1.x + r1.w;

        float r1ot = prevHitBox.y;
        float r1ob = prevHitBox.y + prevHitBox.h;
        float r1ol = prevHitBox.x;
        float r1or = prevHitBox.x + prevHitBox.w;


        float r2t = r2.y;
        float r2b = r2.y + r2.h;
        float r2l = r2.x;
        float r2r = r2.x + r2.w;

        float r2ot = r2t;
        float r2ob = r2b;
        float r2ol = r2l;
        float r2or = r2r;

        if (r1b >= r2t && r1ob <= r2ot) {
          // handle landing collision
          onBottomCollision(&tile);
          position.y = r2t - (size.y - 2);
          onFloor = true;
          velocity.y = 0.0f;
        } else if (r1r >= r2l && r1or <= r2ol) {
          // handle right collision
          onRightCollision(&tile);
          position.x = tile.getRect().x - hitBox.w - hitBox.x;
          velocity.x = 0.0f;
        } else if (r1l <= r2r && r1ol >= r2or) {
          // handle left collision
          onLeftCollision(&tile);
          position.x = tile.getRect().x + tile.getRect().w - hitBox.x;
          velocity.x = 0.0f;
        } else if (r1t <= r2b) {
          // handle top collision
          onTopCollision(&tile);
          position.y = r2b - (size.y - hitBox.h) + 2;
          velocity.y = 0.0f;
        }
      }
    }
  }

  void update(float dt) {
    if (velocity.x > 0.2) {
      velocity.x -= friction;
    } else if (velocity.x < -0.2) {
      velocity.x += (friction * dt) / 10;
    } else {
      velocity.x = 0;
    }

    if (velocity.y < 0.0f) {
      onFloor = false;
    }

    if (!onFloor) {
      velocity.y += (_gravity * dt) / 10;
    }

    if (velocity.y > _maxFallSpeed) {
      velocity.y = _maxFallSpeed;
    }

    Object::update(dt);
  }

  virtual void drawCollisionRectangles(SDL_Renderer* renderer, SDL_Rect origin) {
  }
};
