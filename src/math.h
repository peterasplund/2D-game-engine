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
