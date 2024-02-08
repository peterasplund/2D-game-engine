#include "collidable.h"

collidable::collidable() { }

collidable::collidable(v2 position, SDL_Rect boundingBox) {
  this->boundingBox = boundingBox;
}

bool collidable::checkCollision(SDL_Rect* r, std::vector<std::vector<bool>>* solidTiles, SDL_Rect* outRect) {
  // Don't bother checking if the rect is completely outside the map
  if (r->y + r->h <= 0 || r->x + r->w <= 0) {
    outRect = nullptr;
    return false;
  }

  v2i corners[4] = { 
    { r->x,            r->y },        // Top left
    { r->x + r->w - 1, r->y },        // Top right
    { r->x,            r->y + r->h - 1 }, // Bottom left
    { r->x + r->w - 1, r->y + r->h - 1 }  // Bottom right
  };


  // Run for each of the four corners
  for (int corner = 0; corner < 4; corner++) {
    v2i c = corners[corner];
    // @TODO: use constant for tile_size here instead of 16
    int x = floor(c.x / 16);
    int y = floor(c.y / 16);

    if (y <= solidTiles->size()) {
      if (x <= solidTiles->at(y).size()) {
        if (solidTiles->at(y).at(x) == true) {
          outRect->x = x * 16;
          outRect->y = y * 16;
          outRect->w = 16;
          outRect->h = 16;

          return true;
        }
      }
    }
  }
  outRect = nullptr;
  return false;
}

bool collidable::collideAt(v2 p, SDL_Rect* outRect, v2i subtract) {
  Tilemap* t = EntityManager::Instance()->getTilemap();
  if (t == nullptr) {
    return false;
  }

  auto solidTiles = t->getSolidTiles();
  SDL_Rect r = addBoundingBox(p);
  return checkCollision(&r, &solidTiles, outRect);
}

SDL_Rect collidable::addBoundingBox(v2 p) {
  return SDL_Rect {
    (int)round(p.x) + boundingBox.x,
    (int)round(p.y) + boundingBox.y,
    boundingBox.w,
    boundingBox.h
  };
}

void collidable::update(v2 position) {
  rect = {
    (int)position.x + boundingBox.x,
    (int)position.y + boundingBox.y,
    boundingBox.w,
    boundingBox.h
  };
}

/// Moving with collision resolving
CollisionResponse collidable::moveAndSlide(v2* position, velocity* velocity, float dt) {
  CollisionResponse respnse = { false, false, false, false };
  SDL_Rect collidedWith;
  v2 p = *position;

  if (velocity->y != 0) {
    int pixelsToMove = round((velocity->y / 10) * dt);
    int sign = pixelsToMove > 0 ? 1 : -1;
    pixelsToMove = abs(pixelsToMove);

    int i = 0;
    while (i < pixelsToMove) {
      float newY = p.y + ((i + 1) * sign);
      i += 1;
      if (collideAt({round(p.x), newY}, &collidedWith, { 0, -1 })) {
        if (newY > p.y) {
          position->y = round(collidedWith.y - boundingBox.y - boundingBox.h);
          respnse.top = true;
        }
        else if (newY < p.y) {
          position->y = round(collidedWith.y + collidedWith.h - boundingBox.y);
          respnse.bottom = true;
        }
        velocity->y = 0;
        break;
      }
      else {
        position->y = newY;
      }
    }
  }

  p = *position;

  if (velocity->x != 0) {
    int pixelsToMove = round((velocity->x / 10) * dt);
    int sign = pixelsToMove > 0 ? 1 : -1;
    pixelsToMove = abs(pixelsToMove);

    int i = 0;
    while (i < pixelsToMove) {
      float newX = p.x + ((i + 1) * sign);
      i += 1;
      if (collideAt({newX, round(p.y)}, &collidedWith, { 0, -1 })) {
        if (newX > p.x) {
          position->x = round(collidedWith.x - boundingBox.x - boundingBox.w);
          respnse.left = true;
        }
        else if (newX < p.x) {
          position->x = round(collidedWith.x + collidedWith.w - boundingBox.x);
          respnse.right = true;
        }
        velocity->x = 0;
        break;
      }
      else {
        position->x = newX;
      }
    }
  }

  return respnse;
}
