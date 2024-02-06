#pragma once

#include "stdafx.h"

struct v2
{
  float x;
  float y;

  v2 operator +(const v2& a) {
    return { x + a.x, y + a.y };
  }

  v2 operator -(const v2& a) {
    return { x - a.x, y - a.y };
  }
};

struct v2i
{
  int x;
  int y;

  v2i operator +(const v2i& a) {
    return { x + a.x, y + a.y };
  }

  v2i operator -(const v2i& a) {
    return { x - a.x, y - a.y };
  }
};

// @TODO maybe call this from beforeUpdate
bool checkCollision(SDL_Rect* r, std::vector<std::vector<bool>>* solidTiles, SDL_Rect* outRect) {
    v2i corners[4] = { 
      { r->x,        r->y },        // Top left
      { r->x + r->w, r->y },        // Top right
      { r->x,        r->y + r->h }, // Bottom left
      { r->x + r->w, r->y + r->h }  // Bottom right
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
