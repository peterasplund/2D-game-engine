#pragma once

#include "../math.h"
#include "../tilemap.h"
#include "../entityManager.h"
#include "velocity.h"
#include <algorithm>

// @TODO: use bitfield instead?
struct CollisionResponse {
  bool top;
  bool bottom;
  bool left;
  bool right;

  bool hasCollision() {
    return top || bottom || left || right;
  }
};

class collidable {
  public:
    Rect boundingBox;
    RectF rect;

    collidable();
    collidable(v2f position, Rect boundingBox);
    bool checkCollision(Rect* r, Tilemap* tilemap, Rect* outRect);
    bool collideAt(v2f p, Rect* outRect);
    RectF addBoundingBox(v2f p);
    void update(v2f position);
    CollisionResponse moveAndSlide(v2f* position, velocity* velocity, float dt);
    void moveAndSlide2(v2f position, v2f* velocity, float dt);
};