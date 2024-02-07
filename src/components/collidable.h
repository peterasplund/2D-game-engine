
#pragma once
#include "../stdafx.h"
#include "../math.h"
#include "../entityManager.h"
#include "../tilemap.h"

enum class COLLISION_SIDE {
  NONE,
  TOP,
  LEFT,
  RIGHT,
  BOTTOM,
};

class collidable {
  public:
    SDL_Rect boundingBox;
    SDL_Rect rect;

    collidable() {
    }

    collidable(v2 position, SDL_Rect boundingBox) {
      this->boundingBox = boundingBox;
    }

    bool checkCollision(SDL_Rect* r, std::vector<std::vector<bool>>* solidTiles, SDL_Rect* outRect) {
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

    bool collideAt(v2 p, SDL_Rect* outRect, v2i subtract) {
      Tilemap* t = EntityManager::Instance()->getTilemap();
      if (t == nullptr) {
        return false;
      }

      auto solidTiles = t->getSolidTiles();
      SDL_Rect r = addBoundingBox(p);
      return checkCollision(&r, &solidTiles, outRect);
    }

    SDL_Rect addBoundingBox(v2 p) {
      return SDL_Rect {
        (int)round(p.x) + boundingBox.x,
        (int)round(p.y) + boundingBox.y,
        boundingBox.w,
        boundingBox.h
      };
    }

    void update(v2 position) {
      rect = {
        (int)position.x + boundingBox.x,
        (int)position.y + boundingBox.y,
        boundingBox.w,
        boundingBox.h
      };
    }

    /// Moving with collision resolving
    COLLISION_SIDE moveAndSlideX(v2* position, velocity* velocity, float dt) {
      SDL_Rect collidedWith;
      v2 p = *position;
      COLLISION_SIDE collisionSide = COLLISION_SIDE::NONE;

      if (velocity->x == 0) {
        return collisionSide;
      }

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
            collisionSide = COLLISION_SIDE::LEFT;
          }
          else if (newX < p.x) {
            position->x = round(collidedWith.x + collidedWith.w - boundingBox.x);
            collisionSide = COLLISION_SIDE::RIGHT;
          }
          velocity->x = 0;
          return collisionSide;
        }
        else {
          position->x = newX;
        }
      }

      return collisionSide;
    }

    /// Collision resolving
    COLLISION_SIDE moveAndSlideY(v2* position, velocity* velocity, float dt) {
      SDL_Rect collidedWith;
      v2 p = *position;
      COLLISION_SIDE collisionSide = COLLISION_SIDE::NONE;

      if (velocity->y == 0) {
        return collisionSide;
      }

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
            collisionSide = COLLISION_SIDE::LEFT;
          }
          else if (newY < p.y) {
            position->y = round(collidedWith.y + collidedWith.h - boundingBox.y);
            collisionSide = COLLISION_SIDE::RIGHT;
          }
          velocity->y = 0;
          return collisionSide;
        }
        else {
          position->y = newY;
        }
      }

      return collisionSide;
    }
};