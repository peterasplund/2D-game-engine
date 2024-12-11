#include "collidable.h"
#include "../debugPrinter.h"

struct CollisionInfo {
  int layer;
  int index;
  int tileId;
  float t_hit;
  v2f contactNormal;
  Rect rect;
};

collidable::collidable() {
}

collidable::collidable(v2f position, Rect boundingBox) {
  this->boundingBox = boundingBox;
  this->rect = {position.x + boundingBox.x, position.y + boundingBox.y,
          (float)boundingBox.w, (float)boundingBox.h};
}

std::vector<AbstractGameObject *> collidable::objectExistsAt(RectF rect) {
  auto entities = EntityManager::Instance()->getEntities();
  static std::vector<AbstractGameObject *> response;
  response.clear();

  for (const auto &entity : entities) {
    if (rect.hasIntersection(&entity->_collidable.rect)) {
      response.push_back(entity);
    }
  }

  return response;
}

RectF collidable::addBoundingBox(v2f p) {
  return RectF{p.x + boundingBox.x, p.y + boundingBox.y, (float)boundingBox.w,
               (float)boundingBox.h};
}

void collidable::update(v2f position) {
  rect = {position.x + boundingBox.x, position.y + boundingBox.y,
          (float)boundingBox.w, (float)boundingBox.h};
}

Rect getCollisionAt(RectF r) {
  Level *tilemap = EntityManager::Instance()->getTilemap();
  static std::vector<int> possibleIndices;
  possibleIndices.clear();

  tilemap->getIndicesWithinRect(r, possibleIndices);

  for (uint32_t layer = 0; layer < tilemap->layers.size(); layer++) {
    for (int possibleIdx : possibleIndices) {
      auto layerDef = tilemap->layers[layer].def;
      if (layerDef->type != LayerType::TILES &&
          layerDef->type != LayerType::INT_GRID) {
        continue;
      }

      auto tile = tilemap->layers[layer].tiles[possibleIdx];

      if (!tile.getActive() || !tile.getSolid()) {
        continue;
      }

      Rect otherRect = tilemap->getTileRect(possibleIdx);
      SDL_Rect otherRectSDL = otherRect.to_sdl_rect();
      SDL_Rect rSDL = r.to_sdl_rect();
      DebugPrinter::Instance()->addDebugRect(&otherRect, 255, 255, 0);

      if (SDL_HasIntersection(&rSDL, &otherRectSDL)) {
        return otherRect;
      }
    }
  }

  return {-1, -1, -1, -1};
}

CollisionResponse collidable::moveAndSlide(v2f *position, velocity *velocity,
                                           double dt) {
  CollisionResponse response = {false, false, false, false};
  v2f newPos = *position;
  Rect collidedWith;

  float xValue = velocity->v.x * dt;
  float yValue = velocity->v.y * dt;

  int xInt = std::abs(velocity->v.x > 0 ? floor(xValue) : ceil(xValue));
  int yInt = std::abs(velocity->v.y > 0 ? floor(yValue) : ceil(yValue));

  float xFraction = (std::abs(velocity->v.x) * dt) - xInt;
  float yFraction = (std::abs(velocity->v.y) * dt) - yInt;

  float framePos = 0;
  for (size_t i = 0; i < xInt + 1; i++) {
    if (i == xInt) {
      framePos += xFraction;
    }
    else {
      framePos += 1;
    }

    newPos.x = velocity->v.x > 0 ? position->x + framePos : position->x - framePos;
    RectF r = addBoundingBox(newPos);
    collidedWith = getCollisionAt(r);

    if (collidedWith.w != -1 && collidedWith.h != -1) {
      if (velocity->v.x > 0.0f) {
        newPos.x = floor(collidedWith.x - boundingBox.x - boundingBox.w);
        response.right = true;
      } else {
        newPos.x = floor(collidedWith.right() - boundingBox.x);
        response.left = true;
      }
      velocity->v.x = 0;
      break;
    }
  }

  framePos = 0;
  for (size_t i = 0; i < yInt + 1; i++) {
    if (i == yInt) {
      framePos += yFraction;
    }
    else {
      framePos += 1;
    }

    //printf("framePos: %f\n", framePos);

    newPos.y = velocity->v.y > 0 ? position->y + framePos : position->y - framePos;
    RectF r = addBoundingBox(newPos);
    collidedWith = getCollisionAt(r);

    if (collidedWith.w != -1 && collidedWith.h != -1) {
      if (velocity->v.y > 0.0f) {
        newPos.y = floor(collidedWith.y - boundingBox.y - boundingBox.h);
        response.bottom = true;
      } else {
        newPos.y = floor(collidedWith.bottom() - boundingBox.y);
        newPos.y = floor(collidedWith.bottom() - boundingBox.y);
        response.top = true;
      }
      velocity->v.y = 0;
      break;
    }
  }

  position->x = newPos.x;
  position->y = newPos.y;

  return response;
}
