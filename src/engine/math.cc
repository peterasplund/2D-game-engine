#include "math.h"

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

Rectangle<int, int> group_rects(Rectangle<float, float> r1,
                                Rectangle<float, float> r2) {
  float x = std::min(r1.x, r2.x);
  float y = std::min(r1.y, r2.y);

  float max_right = std::max(r1.right(), r2.right());
  float max_bottom = std::max(r1.bottom(), r2.bottom());

  return {
      (int)x,
      (int)y,
      (int)ceil(max_right - x),
      (int)ceil(max_bottom - y),
  };
}

float lerp(float v0, float v1, float t) {
  return (1 - t) * v0 + t * v1;
}

float easing(float t) {
  if (t <= 0.5f) {
      return 2.0f * t * t;
  }

  t -= 0.5f;

  return 2.0f * t * (1.0f - t) + 0.5f;
}
