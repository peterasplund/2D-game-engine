#pragma once

#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/renderable.h"
#include "../components/velocity.h"
#include "../components/collidable.h"
#include "../components/gravity.h"
#include "../tilemap.h"
#include "../events.h"

void collisionSystem(Tilemap* map, entt::registry* registry, entt::dispatcher* dispatcher) {
  // @TODO: make gravity optional. We only want to update "onFloor" if gravity object
  // @TODO: make velocity optional.
  auto view = registry->view<collidable, position, gravity, velocity>();


  for (auto entity : view) {
    bool floorBelow = false;
    bool hasCollided = false;
    auto &p = view.get<position>(entity);
    auto &v = view.get<velocity>(entity);
    auto &c = view.get<collidable>(entity);
    auto &g = view.get<gravity>(entity);

    SDL_Rect r1 = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };
    SDL_Rect r1below = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h + 1 };

    // do collision resolve
    std::vector<Tile>* tiles = map->getTiles();
    for (int j = 0; j < tiles->size(); j ++) {
      Tile tile = tiles->at(j);

      if (!tile.solid) {
        continue;
      }

      SDL_Rect r2 = { tile.x, tile.y, TILE_SIZE, TILE_SIZE }; 

      // collision
      if (SDL_HasIntersection(&r1, &r2)) {
        if (c.solid) {
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

          // TODO: use second rectangle old state
          float r2ot = r2t;
          float r2ob = r2b;
          float r2ol = r2l;
          float r2or = r2r;

          if (r1r >= r2l && r1or <= r2ol) {
            // onRightCollision(&tile);
            printf("right\n");
            p.x = tile.x - c.rect.w - c.rect.x;
            v.x = 0.0f;
          } else if (r1l <= r2r && r1ol >= r2or) {
            // onLeftCollision(&tile);
            printf("left\n");
            p.x = r2r - c.rect.x;
            v.x = 0.0f;
          } else if (r1b >= r2t && r1ob <= r2ot) {
            // onBottomCollision(&tile);
            printf("landing\n");
            p.y = r2t - c.rect.h - c.rect.y;
            g.onFloor = true;
            v.y = 0.0f;
          } else if (r1t <= r2b) {
            //printf("top\n");
            // onTopCollision(&tile);
            p.y = r2b - c.rect.y;
            v.y = 0.0f;
          }
        }

        if (!hasCollided) {
          dispatcher->enqueue<collisionEvent>({ registry, entity });
        }

        hasCollided = true;
      }

      if (SDL_HasIntersection(&r1below, &r2)) {
        floorBelow = true;
      }
    }

    if (!floorBelow) {
      g.onFloor = false;
    }
  }
}

void setCollisionSystemPrevCollisionBox(entt::registry* registry) {
  auto view = registry->view<collidable, position, gravity, velocity>();

  for (auto entity : view) {
    auto &p = view.get<position>(entity);
    auto &c = view.get<collidable>(entity);
    c.prevRect = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };
  }
}
