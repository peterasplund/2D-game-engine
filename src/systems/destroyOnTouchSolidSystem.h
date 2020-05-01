#pragma once

#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/collidable.h"
#include "../components/destroyOnTouchSolid.h"
#include "../tilemap.h"

void destroyOnTouchSolidSystem(Tilemap* map, entt::registry* registry) {
  auto view = registry->view<collidable, position, destroyOnTouchSolid>();

  for (auto entity : view) {
    bool floorBelow = false;
    auto &p = view.get<position>(entity);
    auto &c = view.get<collidable>(entity);

    SDL_Rect r1 = { (int)p.x + c.rect.x, (int)p.y + c.rect.y, c.rect.w, c.rect.h };

    std::vector<Tile>* tiles = map->getTiles();
    for (int j = 0; j < tiles->size(); j ++) {
      Tile tile = tiles->at(j);

      if (!tile.solid) {
        continue;
      }

      SDL_Rect r2 = { tile.x, tile.y, TILE_SIZE, TILE_SIZE }; 

      if (SDL_HasIntersection(&r1, &r2)) {
        registry->destroy(entity);
        break;
      }
    }
  }
}
