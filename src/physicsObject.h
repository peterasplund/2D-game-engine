#pragma once
#include <SDL2/SDL.h>
#include "object.h"

class PhysicsObject : public Object
{
protected:
  bool onFloor = false;

  SDL_Rect leftRect;
  SDL_Rect rightRect;
  SDL_Rect topRect;
  SDL_Rect bottomRect;

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

  void updateRectPositions() {
    leftRect = getHitBox();
    leftRect.w = 5;
    leftRect.h = leftRect.h / 2.2;
    leftRect.x = leftRect.x;
    leftRect.y = leftRect.y + (leftRect.h / 2);

    rightRect = getHitBox();
    rightRect.w = 5;
    rightRect.h = rightRect.h / 2.2;
    rightRect.x = rightRect.x + getHitBox().w - 5;
    rightRect.y = rightRect.y + (rightRect.h / 2);

    topRect = getHitBox();
    topRect.w = topRect.w / 2;
    topRect.h = 5;
    topRect.x = topRect.x + (topRect.w / 2);
    topRect.y = topRect.y;

    bottomRect = getHitBox();
    //bottomRect.w = bottomRect.w / 2;
    bottomRect.h = 5;
    //bottomRect.x = bottomRect.x + (bottomRect.w / 2);
    bottomRect.y = bottomRect.y + getHitBox().h - 6;
  }

  void resolveCollisions(std::vector<Object> tiles) {
    onFloor = false;

    updateRectPositions();
    
    for (int j = 0; j < tiles.size(); j ++) {
      auto tile = tiles[j];

      if (!tile.getSolid()) {
        continue;
      }

      // collision
      if (Object::intersectsWith(&tile)) {
        if (tile.intersectsWithRect(&topRect) && velocity.y < 0) {
          // handle top collision
          onTopCollision(&tile);
          position.y = tile.getPosition().y + tile.getSize().y - hitBox.y;
          velocity.y = 0.0f;
        } else if (tile.intersectsWithRect(&leftRect) && velocity.x <= 0) {
          // handle left collision
          onLeftCollision(&tile);
          position.x = tile.getRect().x + tile.getRect().w - hitBox.x;
          velocity.x = 0.0f;
        } else if (tile.intersectsWithRect(&rightRect) && velocity.x >= 0) {
          // handle right collision
          onRightCollision(&tile);
          position.x = tile.getRect().x - hitBox.w - hitBox.x;
          velocity.x = 0.0f;
        } else if (tile.intersectsWithRect(&bottomRect) && velocity.y > 0) {
          // handle landing collision
          onBottomCollision(&tile);
          position.y = tile.getPosition().y - size.y + 2;
          onFloor = true;
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
    updateRectPositions();

    SDL_Rect leftRectWithOrigin = { leftRect.x - origin.x, leftRect.y - origin.y, leftRect.w, leftRect.h };
    SDL_Rect rightRectWithOrigin = { rightRect.x - origin.x, rightRect.y - origin.y, rightRect.w, rightRect.h };
    SDL_Rect topRectWithOrigin = { topRect.x - origin.x, topRect.y - origin.y, topRect.w, topRect.h };
    SDL_Rect bottomRectWithOrigin = { bottomRect.x - origin.x, bottomRect.y - origin.y, bottomRect.w, bottomRect.h };

    SDL_SetRenderDrawColor(renderer, 20, 255, 20, 255);
    SDL_RenderFillRect(renderer, &leftRectWithOrigin);
    SDL_SetRenderDrawColor(renderer, 20, 20, 255, 255);
    SDL_RenderFillRect(renderer, &rightRectWithOrigin);
    SDL_SetRenderDrawColor(renderer, 255, 255, 20, 255);
    SDL_RenderFillRect(renderer, &topRectWithOrigin);
    SDL_SetRenderDrawColor(renderer, 255, 20, 20, 255);
    SDL_RenderFillRect(renderer, &bottomRectWithOrigin);
  }
};
