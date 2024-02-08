#pragma once

#include "../stdafx.h"
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
    SDL_Rect boundingBox;
    SDL_Rect rect;

    collidable();
    collidable(v2 position, SDL_Rect boundingBox);
    bool checkCollision(SDL_Rect* r, std::vector<std::vector<bool>>* solidTiles, SDL_Rect* outRect);
    bool collideAt(v2 p, SDL_Rect* outRect);
    SDL_Rect addBoundingBox(v2 p);
    void update(v2 position);
    CollisionResponse moveAndSlide(v2* position, velocity* velocity, float dt);
};