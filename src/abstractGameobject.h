#pragma once

#include "stdafx.h"
#include "components/renderable.h"
#include "components/position.h"
#include "components/velocity.h"
#include "components/gravity.h"
#include "math.h"

class AbstractGameObject {
  public:
    virtual void init(SDL_Renderer* renderer) {
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
    }

    // @TODO: only call this for stuff inside the camera rect
    virtual void draw(SDL_Renderer* renderer, v2 cameraPos) {
      auto &r = this->_renderable;
      auto &p = this->_position;

      SDL_Rect sr = r.textureRect;
      SDL_Rect dr = { (int)round(p.x), (int)round(p.y), sr.w, sr.h };
      //SDL_Rect dr = { (int)round(p.x - cameraPos.x), (int)round(p.y - cameraPos.y), sr.w, sr.h };

      SDL_RenderCopyEx(renderer, r.texture, &sr, &dr, 0, 0, r.textureFlip);
    }

    virtual ~AbstractGameObject() {
    }

  protected:
   renderable _renderable;
   position _position;
   velocity _velocity;
   gravity _gravity;
};
