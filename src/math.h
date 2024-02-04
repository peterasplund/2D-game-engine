#pragma once

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

