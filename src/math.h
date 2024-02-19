#pragma once

#include "SDL.h"

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

  float distanceTo(v2 vec) {
      float dx = x - vec.x;               //calculate the diffrence in x-coordinate
      float dy = y - vec.y;               //calculate the diffrence in y-coordinate
      return sqrt(dx*dx + dy*dy);     //use the distance formula to find the difference
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

struct Rect {
  int x;
  int y;
  int w;
  int h;

  int top() { return y; }
  int bottom() { return y + h; }
  int left() { return x; }
  int right() { return x + w; }

  bool hasIntersection(Rect* other) {
    if (right() >= other->x && x <= other->right()) {
      if (bottom() >= other->y && y <= other->bottom()) {
        return true;
      }
    }

    return false;
  }

  static Rect from_sdl_rect(SDL_Rect r) {
    return Rect {
      r.x,
      r.y,
      r.w,
      r.h,
    };
  }

  SDL_Rect to_sdl_rect() {
    return SDL_Rect {
      this->x,
      this->y,
      this->w,
      this->h,
    };
  }

  // @TODO: place this behind compiler flag
  void debug() {
    printf("(x: %d\ty: %d\tw: %d\th: %d)\n", x, y, w, h);
  }
};
