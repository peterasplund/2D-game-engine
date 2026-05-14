#pragma once

#include "../entityManager.h"
#include "velocity.h"
#include <core/logger.h>
#include <core/map.h>
#include <core/math.h>
#include <optional>

struct CollisionAt {
  int idx;
  Tile tile;
};

// @TODO: use bitfield instead?
/// Each direction corresponds to the ID of the tile it collided with. -1 means
/// no collision.
struct CollisionResponse {
  std::optional<CollisionAt> top = std::nullopt;
  std::optional<CollisionAt> bottom = std::nullopt;
  std::optional<CollisionAt> left = std::nullopt;
  std::optional<CollisionAt> right = std::nullopt;

  bool hasCollision() const {
    return top != std::nullopt || bottom != std::nullopt ||
           left != std::nullopt || right != std::nullopt;
  }

  void print() const {
    LOG_DEBUG("collision (▲%d\t►%d\t▼%d\t◄%d)", top != std::nullopt,
              right != std::nullopt, bottom != std::nullopt,
              left != std::nullopt);
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

  bool dynamicRectVsRect(RectF *r_dynamic, velocity inVelocity, Rect &r_static,
                         v2f &contact_point, v2f &contact_normal,
                         float &contact_time, float dt);

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
