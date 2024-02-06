#pragma once
#include "../stdafx.h"
#include "../components/position.h"
#include "../components/renderable.h"
#include "../components/velocity.h"
#include "../components/collidable.h"
#include "../components/gravity.h"
#include "../components/solid.h"
#include "../events.h"
#include "../abstractGameobject.h"
#include <map>


/*
void collisionSystem() {
  auto entities = registry.group<position, velocity>(entt::get<collidable>);

  entities.each([](entt::entity entity, auto& p, auto& v, const auto& c) {
  //for (auto entity : entities) {
    //auto [c, p, v] = entities.get<collidable, position, velocity>(entity);
  //registry.group<collidable, position, velocity>().each([](auto entity, const auto &c, auto &p, auto &v) {
    auto collidables = registry.view<collidable, position>();
    int calculations = 0;
    int outerCalculations = 0;
    outerCalculations ++;
    bool floorBelow = false;
    bool hasCollided = false;
    auto g = registry.try_get<gravity>(entity);
    auto s = registry.try_get<solid>(entity);
    SDL_Rect r1 = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };

    SDL_Rect r1below = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h + 1 };
    int innerCalculations = 0;

    // Do collision resolve
    for (auto otherEntity : collidables) {
      //innerCalculations ++;
      if (entity == otherEntity) {
        continue;
      }

      const auto op = collidables.get<position>(otherEntity);
      const auto oc = collidables.get<collidable>(otherEntity);
      const auto os = registry.try_get<solid>(otherEntity);
      SDL_Rect r2 = { (int)op.x + oc.rect.x, (int)op.y + oc.rect.y, oc.rect.w, oc.rect.h };
      //calculations ++;

      // collision
      if (SDL_HasIntersection(&r1, &r2)) {
        //if (os->dynamic) {
          float r1t = r1.y;
          float r1b = r1.y + r1.h;
          float r1l = r1.x;
          float r1r = r1.x + r1.w;

          float r1ot = c.prevRect.y;
          float r1ob = c.prevRect.y + c.prevRect.h;
          float r1ol = c.prevRect.x;
          float r1or = c.prevRect.x + c.prevRect.w;

          float r2t = r2.y;
          float r2b = r2.y + r2.h;
          float r2l = r2.x;
          float r2r = r2.x + r2.w;

          float r2ot = oc.prevRect.y;
          float r2ob = oc.prevRect.y + oc.prevRect.h;
          float r2ol = oc.prevRect.x;
          float r2or = oc.prevRect.x + oc.prevRect.w;

          if (s != nullptr && s->dynamic) {
            if (r1r >= r2l && r1or <= r2ol) {
              //printf("right\n");
              if (os != nullptr) {
                dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::right });
                p.x = r2.x - c.rect.w - c.rect.x;
                v.x = 0.0f;
              }
            } else if (r1l <= r2r && r1ol >= r2or) {
              //printf("left\n");
              if (os != nullptr) {
                dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::left });
                p.x = r2r - c.rect.x;
                v.x = 0.0f;
              }
            } else if (r1b >= r2t && r1ob <= r2ot) {
              //printf("landing\n");
              dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::bottom });
              if (os != nullptr) {
                p.y = r2t - c.rect.h - c.rect.y;
                if (g != nullptr) {
                  g->onFloor = true;
                }
                v.y = 0.0f;
              }
            } else if (r1t <= r2b) {
              //printf("top\n");
              dispatcher.trigger<collisionSideEvent>({ entity, otherEntity, CollisionDirection::top });
              if (os != nullptr) {
                p.y = r2b - c.rect.y;
                v.y = 0.0f;
              }
            }
          }
        //}

        if (!hasCollided && registry.valid(otherEntity)) {
          dispatcher.trigger<collisionEvent>({ entity, otherEntity });
        }

        hasCollided = true;
      }

      if (SDL_HasIntersection(&r1below, &r2)) {
        floorBelow = true;
        auto ov = registry.try_get<velocity>(otherEntity);
        if (s != nullptr && ov != nullptr && s->dynamic == true && os != nullptr && os->dynamic == false) {
          v.x = ov->x;
        }
      }
    }

    if (!floorBelow && g != nullptr) {
      g->onFloor = false;
    }

    //printf("collision inner calculations: %d\n", innerCalculations);
  });

  //printf("collision calculations: %d\n", calculations);
  //printf("collision outer calculations: %d\n", outerCalculations);
}
*/

/*
void setCollisionSystemPrevCollisionBox() {
  auto view = registry.view<collidable, position, velocity>();

  for (auto entity : view) {
    auto &p = view.get<position>(entity);
    auto &c = view.get<collidable>(entity);
    c.prevRect = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };
  }
}

void initCollisionSystem() {
  auto view = registry.view<collidable, position>();

  for (auto entity : view) {
    auto &p = view.get<position>(entity);
    auto &c = view.get<collidable>(entity);
    c.prevRect = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };
  }
}
*/

class CollisionHandler {
  public:
    void init(std::vector<AbstractGameObject*>* objects) {
      // set prev rect for each object
      for(AbstractGameObject* o : *objects) {
        v2 p = o->getPosition();
        SDL_Rect* r = o->getRectPointer();
        o->_collidable.prevRect = { (int)p.x + r->x, (int)p.y + r->y, r->w, r->h };
      }
    }

    void beforeUpdate(std::vector<AbstractGameObject*>* objects, std::vector<std::vector<bool>>* solidTiles, SDL_Renderer* renderer) {
      // Check for collisions and trigger events if so
      for(AbstractGameObject* o : *objects) {
        if (!o->getListensForCollisions()) {
          continue;
        }

        SDL_Rect* r = o->getRectPointer();
        v2i corners[4] = { 
          { r->x,        r->y },        // Top left
          { r->x + r->w, r->y },        // Top right
          { r->x,        r->y + r->h }, // Bottom left
          { r->x + r->w, r->y + r->h }  // Bottom right
        };

        SDL_Rect c1 { corners[0].x, corners[0].y, 4, 4 };
        SDL_RenderDrawRect(renderer, &c1);
        SDL_Rect c2 { corners[1].x -4, corners[1].y, 4, 4 };
        SDL_RenderDrawRect(renderer, &c2);
        SDL_Rect c3 { corners[2].x, corners[2].y -4, 4, 4 };
        SDL_RenderDrawRect(renderer, &c3);
        SDL_Rect c4 { corners[3].x -4, corners[3].y -4, 4, 4 };
        SDL_RenderDrawRect(renderer, &c4);

        v2i cornerCollisions[4];
        bool gotCollision = false;
        SDL_Rect other;

        // Run for each of the four corners
        for (int corner = 0; corner < 4; corner++) {
          v2i c = corners[corner];
          // @TODO: use constant for tile_size here instead of 16
          int x = floor(c.x / 16);
          int y = floor(c.y / 16);

          if (y <= solidTiles->size()) {
            if (x <= solidTiles->at(y).size()) {
              //if (true) {
              if (solidTiles->at(y).at(x) == true) {
                // We have an collision
                cornerCollisions[corner] = { x, y};
                gotCollision = true;
                other = { x * 16, y * 16, 16, 16};
                SDL_Rect c1 { other.x, other.y, other.w, other.h };
                SDL_RenderDrawRect(renderer, &c1);

              }
              else {
                cornerCollisions[corner] = { -1, -1 };
              }
            }
          }
        }

        if (gotCollision) {
          o->onSolidCollision(other, cornerCollisions);
        }
        else {
        }


        // @TODO: loop through all objects again, check SDL_HasIntersection and 
        // trigger events based on collision side, collision response etc.

      }
    }

    void afterUpdate(std::vector<AbstractGameObject*>* objects) {
      /*
      // set prev rect for each object
      for(AbstractGameObject* o : *objects) {
        // o->getRectPointer()->x = o->getPositionPointer()->x; o->getRectPointer()->y = o->getPositionPointer()->y;
        v2 p = o->getPosition();
        SDL_Rect* r = o->getRectPointer();
        o->_collidable.prevRect = { (int)p.x + r->x, (int)p.y + r->y, r->w, r->h };
      }
      */
    }
};
