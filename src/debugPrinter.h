#pragma once

#include "stdafx.h"
#include "math.h"
#include "abstractGameobject.h"
#include "entityManager.h"

namespace debugPrinter {
  void drawHitboxes(SDL_Renderer* renderer, Rect camera) {
    for(const auto &obj : *EntityManager::Instance()->getEntities()) {
      Rect objRect = obj->getRect();
      if (objRect.hasIntersection(&camera)) {
        SDL_Rect hitbox = obj->_collidable.boundingBox.to_sdl_rect();
        SDL_Rect rect = { 
          (int)floor(obj->_position.x) + hitbox.x - camera.x,
          (int)floor(obj->_position.y) + hitbox.y - camera.y,
          hitbox.w,
          hitbox.h,
        };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
        obj->draw(renderer, { (float)camera.x, (float)camera.y });
      }
    }
  }
}
