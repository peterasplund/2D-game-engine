#include "debugPrinter.h"

void DebugPrinter::addDebugRect(Rect* rect, Uint8 r, Uint8 g, Uint8 b) {
  if (!*debugRectangles) {
    return;
  }

  debug_rects.push_back({
      r,
      g,
      b,
      rect->to_sdl_rect(),
  });
}
  
void DebugPrinter::draw(SDL_Renderer* renderer) {
  if (!*debugRectangles) {
    return;
  }

  for(auto r : debug_rects) {
    SDL_SetRenderDrawColor(renderer, r.r, r.g, r.b, 255);
    SDL_RenderDrawRect(renderer, &r.rect);
  }

  debug_rects.clear();
}

void DebugPrinter::drawHitboxes(SDL_Renderer* renderer, Rect camera) {
  if (!*debugRectangles) {
    return;
  }

  for(const auto &obj : *EntityManager::Instance()->getEntities()) {
    Rect objRect = obj->getRect();
    if (objRect.hasIntersection(&camera)) {
      SDL_Rect hitbox = obj->_collidable.boundingBox.to_sdl_rect();
      SDL_Rect rect = { 
        (int)round(obj->_position.x) + hitbox.x - camera.x,
        (int)round(obj->_position.y) + hitbox.y - camera.y,
        hitbox.w,
        hitbox.h,
      };
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &rect);
      obj->draw(renderer, { (float)camera.x, (float)camera.y });
    }
  }
}
