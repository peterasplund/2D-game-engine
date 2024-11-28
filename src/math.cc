#include "math.h"
#include "logger.h"

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

/*
void Rectangle<int, int>::debug() {
  LOG_INFO("(x: %f\ty: %f\tw: %f\th: %f)\n", x, y, w, h);
}

void Rectangle<int, int>::debugInt() {
  LOG_INFO("(x: %d\ty: %d\tw: %d\th: %d)\n", x, y, w, h);
}
*/
