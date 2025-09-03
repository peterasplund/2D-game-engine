#pragma once

#include <core/math.h>
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

    for(auto t : tilesAhead) {
      if (t.tile.getSolid()) {
        return true;
      }
    }

    if (useGravity) {
      auto tilesBelow = collidable->tileExistsAtF(rectBelow);

      bool hasTileBelow = false;
      for(auto t : tilesBelow) {
        if (t.tile.getSolid()) {
          hasTileBelow = true;
        }
      }

      if (!hasTileBelow) {
        return true;
      }
    }

    return false;
  }
};
