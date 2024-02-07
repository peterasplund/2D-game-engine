#pragma once

#include "stdafx.h"
#include "components/renderable.h"
#include "components/velocity.h"
#include "components/gravity.h"
#include "components/collidable.h"
#include "math.h"
#include "events.h"
#include "entityManager.h"

void debugRect(SDL_Rect r) {
  printf("(x: %d\ty: %d\tw: %d\th: %d)\n", r.x, r.y, r.w, r.h);
}

class AbstractGameObject {
  public:
   collidable _collidable;
   v2 _position;
   velocity _velocity;
   gravity _gravity;

  virtual void init(SDL_Renderer* renderer) {
    _collidable = collidable(_position, _renderable.textureRect);
  }


  virtual void update(float dt) {
    CollisionResponse collisionResponse = _collidable.moveAndSlide(&_position, &_velocity, dt);
    _gravity.update(&_position, &_velocity, dt);
    _collidable.update(_position);

    auto &v = _velocity;
    auto &p = _position;

    if (collisionResponse.hasCollision()) {
      printf("collision: (top: %b, right: %b, bottom: %b, left: %b)\n", 
        collisionResponse.top, 
        collisionResponse.right, 
        collisionResponse.bottom, 
        collisionResponse.left 
      );
    }

    // Friction
    if (v.x > 0.2) {
      v.x -= v.friction;
    } else if (v.x < -0.2) {
      v.x += (v.friction * dt) / 10;
    } else {
      v.x = 0;
    }

    // Handle Y friction if we don't have gravity
    if (_gravity.entityGravity == 0.0f) {
      if (v.y > 0.2) {
        v.y -= v.friction;
      } else if (v.y < -0.2) {
        v.y += (v.friction * dt) / 10;
      } else {
        v.y = 0;
      }
    }

    //printf("vel: %f\n", _velocity.y);
    if (_gravity.entityGravity != 0.0f) {
      if (collisionResponse.bottom) {
        _velocity.y = 0;
      }
      if (collisionResponse.top) {
        //printf("on floor\n");
        _gravity.onFloor = true;
        _velocity.y = 0;
      }
      else {
        //_gravity.onFloor = false;
      }
    }

  }

  // @TODO: only call this for stuff inside the camera rect
  virtual void draw(SDL_Renderer* renderer, v2 offset) {
    // @TODO: add vectors instead
    _renderable.render(renderer, this->_position - offset);
  }

  virtual ~AbstractGameObject() {
  }

  virtual void handleEvent(SDL_Event* event) {
  }

  void setListenForCollisions() {
    _listenForCollisions = true;
  }

  virtual bool contains(SDL_Rect other) {
    return SDL_HasIntersection(&_collidable.rect, &other);
  }

  virtual bool contains(AbstractGameObject other) {
    return SDL_HasIntersection(&_collidable.rect, &other._collidable.rect);
  }

  virtual void onSolidCollision(SDL_Rect other) {
  }

  SDL_Rect getRect() {
    return {
      (int)_position.x,
      (int)_position.y,
      _collidable.rect.w,
      _collidable.rect.h,
    };
  }

  SDL_Rect* getRectPointer() {
    return &this->_collidable.rect;
  }

  v2 getPosition() {
    return this->_position;
  }

  v2* getPositionPointer() {
    return &this->_position;
  }

  bool getListensForCollisions() {
    return this->_listenForCollisions;
  }

  protected:
   /// Will be able to listen to other collisions.
   bool _listenForCollisions = false;

   Renderable _renderable;
};
