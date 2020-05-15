#pragma once

#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/renderable.h"
#include "../components/velocity.h"
#include "../components/collidable.h"
#include "../components/gravity.h"
#include "../components/solid.h"
#include "../events.h"

void collisionSystem(entt::registry* registry, entt::dispatcher* dispatcher) {
  auto entities = registry->group<position, velocity>(entt::get<collidable>);

  entities.each([registry, dispatcher](entt::entity entity, auto& p, auto& v, const auto& c) {
  //for (auto entity : entities) {
    //auto [c, p, v] = entities.get<collidable, position, velocity>(entity);
  //registry->group<collidable, position, velocity>().each([registry, dispatcher](auto entity, const auto &c, auto &p, auto &v) {
    auto collidables = registry->view<collidable, solid, position>();
    int calculations = 0;
    int outerCalculations = 0;
    outerCalculations ++;
    bool floorBelow = false;
    bool hasCollided = false;
    auto g = registry->try_get<gravity>(entity);
    auto s = registry->try_get<solid>(entity);
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
      const auto os = collidables.get<solid>(otherEntity);
      SDL_Rect r2 = { (int)op.x + oc.rect.x, (int)op.y + oc.rect.y, oc.rect.w, oc.rect.h };
      //calculations ++;

      // collision
      if (SDL_HasIntersection(&r1, &r2)) {
        //if (os.dynamic) {
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
              dispatcher->enqueue<collisionSideEvent>({ registry, entity, otherEntity, CollisionDirection::right });
              p.x = r2.x - c.rect.w - c.rect.x;
              v.x = 0.0f;
            } else if (r1l <= r2r && r1ol >= r2or) {
              //printf("left\n");
              dispatcher->enqueue<collisionSideEvent>({ registry, entity, otherEntity, CollisionDirection::left });
              p.x = r2r - c.rect.x;
              v.x = 0.0f;
            } else if (r1b >= r2t && r1ob <= r2ot) {
              //printf("landing\n");
              dispatcher->enqueue<collisionSideEvent>({ registry, entity, otherEntity, CollisionDirection::bottom });
              p.y = r2t - c.rect.h - c.rect.y;
              if (g != nullptr) {
                g->onFloor = true;
              }
              v.y = 0.0f;
            } else if (r1t <= r2b) {
              //printf("top\n");
              dispatcher->enqueue<collisionSideEvent>({ registry, entity, otherEntity, CollisionDirection::top });
              p.y = r2b - c.rect.y;
              v.y = 0.0f;
            }
          }
        //}

        if (!hasCollided) {
          dispatcher->enqueue<collisionEvent>({ registry, entity, otherEntity });
        }

        hasCollided = true;
      }

      if (SDL_HasIntersection(&r1below, &r2)) {
        floorBelow = true;
        auto ov = registry->try_get<velocity>(otherEntity);
        if (s != nullptr && ov != nullptr && s->dynamic == true && os.dynamic == false) {
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

void setCollisionSystemPrevCollisionBox(entt::registry* registry) {
  auto view = registry->view<collidable, position, velocity>();

  for (auto entity : view) {
    auto &p = view.get<position>(entity);
    auto &c = view.get<collidable>(entity);
    c.prevRect = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };
  }
}

void initCollisionSystem(entt::registry* registry) {
  auto view = registry->view<collidable, position>();

  for (auto entity : view) {
    auto &p = view.get<position>(entity);
    auto &c = view.get<collidable>(entity);
    c.prevRect = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };
  }
}

