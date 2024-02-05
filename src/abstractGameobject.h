#pragma once

#include "stdafx.h"
#include "components/renderable.h"
#include "components/velocity.h"
#include "components/gravity.h"
#include "components/collidable.h"
#include "math.h"
#include "events.h"

class AbstractGameObject {
  public:
   collidable _collidable;

    virtual void init(SDL_Renderer* renderer) {
      _collidable.rect = _renderable.textureRect;
      _collidable.prevRect = _collidable.rect;
    }

    virtual void update(float dt) {
      auto &v = this->_velocity;
      auto &p = this->_position;

      p.x += (v.x / 10) * dt;
      p.y += (v.y / 10) * dt;

      if (v.x > 0.2) {
        v.x -= v.friction;
      } else if (v.x < -0.2) {
        v.x += (v.friction * dt) / 10;
      } else {
        v.x = 0;
      }
      //_collidable.rect.w = _collidable.rect.w; // not really correct but wth
      //_collidable.rect.h = _collidable.rect.h; // not really correct but wth

      //_collidable.rect.x = _position.x;
      //_collidable.rect.y = _position.y;
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

    virtual void onSolidCollision(AbstractGameObject* other) {
    }

    virtual void onSolidCollision(SDL_Rect other, v2i cornerCollisions[4]) {
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

   v2 _position;
   velocity _velocity;
   gravity _gravity;
};
