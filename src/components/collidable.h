#pragma once

#include "../math.h"
#include "../map.h"
#include "../entityManager.h"
#include "velocity.h"

// @TODO: use bitfield instead?
/// Each direction corresponds to the ID of the tile it collided with. -1 means no collision.
struct CollisionResponse {
  int top = -1;
  int bottom = -1;
  int left = -1;
  int right = -1;

  bool hasCollision() {
    return top != -1 || bottom != -1 || left != -1 || right != -1;
  }


  void print() {
    printf("top: %d\t right: %d\t bottom: %d\tleft: %d\n", top, right, bottom, left);
  }
};

struct TileExistsAtResponse {
  int layerId;
  int tileId;
  int tilesetId;
  Tile tile;
  Rect rect;
};

class collidable {
  public:
    Rect boundingBox;
    RectF rect;

    collidable();
    collidable(v2f position, Rect boundingBox);
    bool checkCollision(Rect* r, Level* tilemap, Rect* outRect);
    std::vector<TileExistsAtResponse> tileExistsAt(RectF rect);
    std::vector<AbstractGameObject*> objectExistsAt(RectF rect);
    RectF addBoundingBox(v2f p);
    void update(v2f position);
    CollisionResponse moveAndSlide(v2f* position, velocity* velocity, float dt);
};