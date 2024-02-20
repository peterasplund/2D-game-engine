#pragma once

#include "../math.h"
#include "../tilemap.h"
#include "../entityManager.h"
#include "velocity.h"

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
    Rect rect;

    collidable();
    collidable(v2 position, Rect boundingBox);
    bool checkCollision(Rect* r, Tilemap* tilemap, Rect* outRect);
    bool collideAt(v2 p, Rect* outRect);
    Rect addBoundingBox(v2 p);
    void update(v2 position);
    CollisionResponse moveAndSlide(v2* position, velocity* velocity, float dt);
};