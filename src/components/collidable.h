#pragma once

#include "../engine/logger.h"
#include "../engine/map.h"
#include "../engine/math.h"
#include "../entityManager.h"
#include "velocity.h"

// @TODO: use bitfield instead?
/// Each direction corresponds to the ID of the tile it collided with. -1 means
/// no collision.
struct CollisionResponse {
  int top = -1;
  int bottom = -1;
  int left = -1;
  int right = -1;

  bool hasCollision() {
    return top != -1 || bottom != -1 || left != -1 || right != -1;
  }

  void print() {
    LOG_INFO("top: %d\t right: %d\t bottom: %d\tleft: %d", top, right, bottom,
             left);
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
  bool checkCollision(Rect *r, Level *tilemap, Rect *outRect);
  std::vector<TileExistsAtResponse> tileExistsAtF(RectF rect) {
    static std::vector<TileExistsAtResponse> response;
    response.clear();

    Level *t = EntityManager::Instance()->getTilemap();
    static std::vector<int> possibleIndices;
    possibleIndices.clear();

    Rect r = {(int)round(rect.x), (int)round(rect.y), (int)round(rect.w),
              (int)round(rect.h)};
    t->getIndicesWithinRect(r, possibleIndices);

    for (uint32_t layerId = 0; layerId < t->layers.size(); layerId++) {
      if (t->layers[layerId].tiles.size() == 0) {
        continue;
      }

      for (int possibleIdx : possibleIndices) {
        Tile tile = t->layers[layerId].tiles[possibleIdx];

        if (tile.getActive()) {
          int tileId = possibleIdx;
          Rect r = t->getTileRect(possibleIdx);

          if (rect.hasIntersection(&r)) {
            int tilesetId = t->layers[layerId].def->tilesetId;

            response.push_back(TileExistsAtResponse{
                (int)layerId,
                tileId,
                tilesetId,
                tile,
                r,
            });
          }
        }
      }
    }

    return response;
  }

  std::vector<TileExistsAtResponse> tileExistsAtI(Rect rect) {
    static std::vector<TileExistsAtResponse> response;
    response.clear();

    Level *t = EntityManager::Instance()->getTilemap();
    static std::vector<int> possibleIndices;
    possibleIndices.clear();

    t->getIndicesWithinRect(rect, possibleIndices);

    for (uint32_t layerId = 0; layerId < t->layers.size(); layerId++) {
      if (t->layers[layerId].tiles.size() == 0) {
        continue;
      }

      for (int possibleIdx : possibleIndices) {
        Tile tile = t->layers[layerId].tiles[possibleIdx];

        if (tile.getActive()) {
          int tileId = possibleIdx;
          Rect r = t->getTileRect(possibleIdx);

          if (rect.hasIntersection(&r)) {
            int tilesetId = t->layers[layerId].def->tilesetId;

            response.push_back(TileExistsAtResponse{
                (int)layerId,
                tileId,
                tilesetId,
                tile,
                r,
            });
          }
        }
      }
    }

    return response;
  }

  std::vector<AbstractGameObject *> objectExistsAt(RectF rect);
  RectF addBoundingBox(v2f p);
  void update(v2f position);
  CollisionResponse moveAndSlide(v2f *position, velocity *velocity, double dt);
};
