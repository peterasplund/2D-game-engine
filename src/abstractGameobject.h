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

    bool collideAt(v2 p, SDL_Rect* outRect) {
      Tilemap* t = EntityManager::Instance()->getTilemap();
      auto solidTiles = t->getSolidTiles();
      SDL_Rect r = _collidable.addBoundingBox(p);
      return checkCollision(&r, &solidTiles, outRect);
    }

    virtual void update(float dt) {
      auto &v = this->_velocity;
      auto &p = this->_position;

      // Collision handling
      if (v.x != 0) {
        SDL_Rect collidedWith;
        float nx = round(p.x + ((v.x / 10) * dt));
        if (collideAt({nx, p.y}, &collidedWith)) {
          // printf("collided with:\n"); debugRect(collidedWith);

          // One pixel from solid
          p.x = nx > p.x 
            ? collidedWith.x - _collidable.boundingBox.x - _collidable.boundingBox.w - 1
            : collidedWith.x + collidedWith.w - _collidable.boundingBox.x;
          printf("px: %d\n", (int)p.x);
        }
        else {
          p.x = nx;
        }
      }

      if (v.y != 0) {
        SDL_Rect collidedWith;
        float ny = round(p.y + ((v.y / 10) * dt));
        if (collideAt({p.x, ny}, &collidedWith)) {
          // printf("collided with:\n"); debugRect(collidedWith);

          // One pixel from solid
          p.y = ny > p.y 
            ? collidedWith.y - _collidable.boundingBox.y - _collidable.boundingBox.h - 1
            : collidedWith.y + collidedWith.h - _collidable.boundingBox.y;
          printf("py: %d\n", (int)p.x);
        }
        else {
          p.y = ny;
        }
      }

      // Friction
      if (v.x > 0.2) {
        v.x -= v.friction;
      } else if (v.x < -0.2) {
        v.x += (v.friction * dt) / 10;
      } else {
        v.x = 0;
      }

      _collidable.update(_position);
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

    /// Returns position
    void resolveCollision(SDL_Rect* other, v2i cornerCollisions[4]) {
      return;
      //CollisionResolve resolve;

      auto c = this->_collidable;

      SDL_Rect r1 = this->_collidable.rect;
      SDL_Rect r2 = *other;

      SDL_Rect r1p = this->_collidable.prevRect;

      //SDL_Rect selfBelow = { r1.x + r1.x, r1.y + r1.y, r1.w, r1.h + 1 };
      //bool floorBelow = false;

      float r1t = r1.y;
      float r1b = r1.y + r1.h;
      float r1l = r1.x;
      float r1r = r1.x + r1.w;

      float r1ot = r1p.y;
      float r1ob = r1p.y + r1p.h;
      float r1ol = r1p.x;
      float r1or = r1p.x + r1p.w;

      float r2t = r2.y;
      float r2b = r2.y + r2.h;
      float r2l = r2.x;
      float r2r = r2.x + r2.w;

      char hor = (r1.x < r1p.x) ? 'r' : 'l';
      char ver = (r1.y < r1p.y) ? 'u' : 'd';

      //debugRect(r2);

      float r2ol = r2l;
      float r2or = r2r;
      float r2ot = r2t;

      //if (r1r >= r2l && r1or <= r2ol) {
      if (hor == 'l' && cornerCollisions[0].x == -1) {
        printf("right\n");
        //dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::right });
        this->_position.x = r2.x - _collidable.boundingBox.w - 14;
        this->_velocity.x = 0.0f;
      } else if (hor == 'r' && cornerCollisions[1].x == -1) {
        printf("left\n");
        //dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::left });
        this->_position.x = r2r - _collidable.boundingBox.x + 1;
        this->_velocity.x = 0.0f;
      } else if (ver == 'u' && cornerCollisions[2].x == -1) {
        //printf("landing\n");
        //dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::bottom });
        this->_position.y = r2b - 14;// - _collidable.boundingBox.h;
        //g->onFloor = true;
        this->_velocity.y = 0.0f;
      } else if (ver == 'd' && cornerCollisions[0].x == -1) {
        //printf("top\n");
        //dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::top });
        this->_position.y = r2t - _collidable.boundingBox.h - 15;
        this->_velocity.y = 0.0f;
      }

      /*
      if (SDL_HasIntersection(&selfBelow, &r2)) {
        floorBelow = true;
        auto ov = registry.try_get<velocity>(otherEntity);
        if (s != nullptr && ov != nullptr && s->dynamic == true && os != nullptr && os->dynamic == false) {
          v.x = ov->x;
        }
      }
      */
    }

    virtual void onSolidCollision(AbstractGameObject* other) {
      //this->resolveCollision(&other->_collidable.rect);
    }

    virtual void onSolidCollision(SDL_Rect other, v2i cornerCollisions[4]) {
      printf("collision\n");
      this->resolveCollision(&other, cornerCollisions);
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

  SDL_Rect* getPrevRectPointer() {
    return &this->_collidable.prevRect;
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
