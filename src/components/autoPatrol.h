#pragma once

#include "../engine/math.h"
#include "../abstractGameobject.h"

struct AutoPatrol {
  bool shouldTurn(collidable* collidable, Direction dir, bool useGravity, int howEarly = 1) {
    RectF rectAhead = {
      collidable->rect.x,
      collidable->rect.y,
      collidable->rect.w,
      collidable->rect.h -  1,
    };

    RectF rectBelow = {
      collidable->rect.x,
      collidable->rect.y,
      collidable->rect.w,
      collidable->rect.h + 1,
    };

    if (dir == Direction::RIGHT) {
      rectAhead.x += howEarly;
      rectBelow.x += collidable->rect.w + howEarly;
    }
    else {
      rectAhead.x -= howEarly;
      rectBelow.x -= collidable->rect.w - howEarly;
    }

    auto tilesAhead = collidable->tileExistsAtF(rectAhead);

    if (tilesAhead.size() > 0) {
      return true;
    }

    // @NOTE: This isn't working right now. Fix
    if (useGravity) {
      auto tilesBelow = collidable->tileExistsAtF(rectBelow);
      if (tilesBelow.size() == 0) {
        return true;
      }
    }

    return false;
  }
};
