#include "debugPrinter.h"

void DebugPrinter::addDebugRect(Rect* rect, Uint8 r, Uint8 g, Uint8 b) {
  if (!*debugRectangles) {
    return;
  }

  debug_rects.push_back({
      r,
      g,
      b,
      *rect,
  });
}

void DebugPrinter::draw(Renderer* renderer) {
  if (!*debugRectangles) {
    return;
  }

  for(auto r : debug_rects) {
    renderer->setColor(r.r, r.g, r.b, 255);
    renderer->renderRect(&r.rect);
  }

  debug_rects.clear();

  if (*debugRectangles) {
    for(const auto &obj : EntityManager::Instance()->getEntities()) {
      RectF r = obj->_collidable.addBoundingBox(obj->_position);
      SDL_Rect hitbox = r.to_sdl_rect();
      Rect rect = { 
        (int)round(hitbox.x),
        (int)round(hitbox.y),
        hitbox.w,
        hitbox.h,
      };
      renderer->setColor(255, 0, 0, 255);
      renderer->renderRect(&rect);
    }
  }
}
