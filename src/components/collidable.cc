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

collidable::collidable() { }

collidable::collidable(v2f position, Rect boundingBox) {
  this->boundingBox = boundingBox;
}

// Calculate "Near time" and "Far time"
// https://youtu.be/8JJ-4JgR7Dg?t=1813
bool rayVsRect(v2f& ray_origin, v2f& ray_dir, RectF* target,
    v2f& contact_point, v2f& contact_normal, float& t_hit_near) {
  contact_normal = { 0,0 };
  contact_point = { 0,0 };

  // Cache division
  v2f invdir = 1.0f / ray_dir;
  v2i targetPos = target->pos();
  v2i targetSize = target->size();

  // Calculate intersections with rectangle bounding axes
  v2f t_near = (targetPos - ray_origin) * invdir;
  v2f t_far = (targetPos + targetSize - ray_origin) * invdir;

  if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
  if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

  // Sort distances
  if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
  if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

  // Early rejection		
  if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

  // Closest 'time' will be the first contact
  t_hit_near = std::max(t_near.x, t_near.y);

  // Furthest 'time' is contact on opposite side of target
  float t_hit_far = std::min(t_far.x, t_far.y);

  // Reject if ray direction is pointing away from object
  if (t_hit_far < 0)
    return false;

  // Contact point of collision from parametric line equation
  contact_point = ray_origin + t_hit_near * ray_dir;

  if (t_near.x > t_near.y) {
    if (invdir.x < 0) {
      contact_normal = { 1, 0 };
    }
    else {
      contact_normal = { -1, 0 };
    }
  }
  else if (t_near.x < t_near.y) {
    if (invdir.y < 0) {
      contact_normal = { 0, 1 };
    }
    else {
      contact_normal = { 0, -1 };
    }
  }

  return true;
}

bool dynamicRectVsRect(
    RectF* r_dynamic, velocity inVelocity,
    Rect& r_static, v2f& contact_point,
    v2f& contact_normal, float& contact_time, float dt) {
  if (inVelocity.v.x == 0.0f && inVelocity.v.y == 0.0f) {
    return false;
  }

  RectF expanded_target;
  expanded_target.setPos(r_static.pos() - r_dynamic->size() / 2);
  expanded_target.setSize(r_static.size() + r_dynamic->size());

  v2f ray_origin = r_dynamic->pos() + r_dynamic->size() / 2;

  v2f velocity = inVelocity.v * dt;

  if (rayVsRect(
        ray_origin,
        velocity,
        &expanded_target,
        contact_point,
        contact_normal,
        contact_time)) {
    return (contact_time >= 0.0f && contact_time < 1.0f);
  }

  return false;
}

std::vector<TileExistsAtResponse> collidable::tileExistsAt(RectF rect) {
  std::vector<TileExistsAtResponse> response;
  Tilemap* t = EntityManager::Instance()->getTilemap();
  for(int layerId = 0; layerId < t->getLayers()->size(); layerId++) {
    std::vector<int> tiles = t->getIndicesWithinRect(rect, layerId);
    for(int possibleIdx : tiles) {
      int tileId = 0;
      tileId = t->getLayers()->at(layerId).tiles.at(possibleIdx);
      Rect r = t->getTileRect(layerId, possibleIdx);

      RectF rF = {
        (float)r.x,
        (float)r.y,
        (float)r.w,
        (float)r.h
      };

      if (rect.hasIntersection(&rF)) {
        response.push_back(TileExistsAtResponse {
          layerId,
          tileId,
          rF,
        });
      }
    }
  }

  return response;
}

std::vector<std::shared_ptr<AbstractGameObject>> objectExistsAt(RectF rect) {
  auto entities = EntityManager::Instance()->getEntities();
  std::vector<std::shared_ptr<AbstractGameObject>> response;

  for(const auto &entity : entities) {
    if (rect.hasIntersection(&entity->_collidable.rect)) {
      response.push_back(entity);
    }
  }

  return response;
}

RectF collidable::addBoundingBox(v2f p) {
  return RectF {
    p.x + boundingBox.x,
    p.y + boundingBox.y,
    (float)boundingBox.w,
    (float)boundingBox.h
  };
}

void collidable::update(v2f position) {
  rect = {
    position.x + boundingBox.x,
    position.y + boundingBox.y,
    (float)boundingBox.w,
    (float)boundingBox.h
  };
}

Rect getCollisionAt(RectF r) {
  bool collided = false;

  Tilemap* tilemap = EntityManager::Instance()->getTilemap();
  auto layers = tilemap->getLayers();

  for (int layer = 0; layer < layers->size(); layer++) {
    std::vector<int> possibleIndices = tilemap->getIndicesWithinRect(r, layer);
    for (int possibleIdx : possibleIndices) {
      int tileId = 0;
      tileId = layers->at(layer).tiles.at(possibleIdx);

      if (tileId == 0) {
        continue;
      }

      TileData* tileData = EntityManager::Instance()->getTilemap()->getTileData(tileId);

      if (!tileData->solid) {
        continue;
      }

      Rect otherRect = tilemap->getTileRect(layer, possibleIdx);
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

CollisionResponse collidable::moveAndSlide(v2f* position, velocity* velocity, float dt) {
  CollisionResponse respnse = { false, false, false, false };
  v2 newPos = *position + velocity->v * dt;

  Tilemap* tilemap = EntityManager::Instance()->getTilemap();
  auto layers = tilemap->getLayers();
  Rect collidedWith;

  if (velocity->v.y != 0) {
    RectF r = addBoundingBox({position->x, newPos.y});
    collidedWith = getCollisionAt(r);

    if (collidedWith.w != -1 && collidedWith.h != -1) {
      if (velocity->v.y > 0.0f) {
        newPos.y = floor(collidedWith.y - boundingBox.y - boundingBox.h);
        respnse.top = true;
      }
      else {
        newPos.y = floor(collidedWith.bottom() - boundingBox.y);
        respnse.bottom = true;
      }
      velocity->v.y = 0.0f;
    }
  }

  if (velocity->v.x != 0) {
    Rect collidedWith;
    RectF r = addBoundingBox({newPos.x, position->y});
    collidedWith = getCollisionAt(r);

    if (collidedWith.w != -1 && collidedWith.h != -1) {
      if (velocity->v.x > 0.0f) {
        newPos.x = floor(collidedWith.x - boundingBox.x - boundingBox.w);
        respnse.right = true;
      }
      else {
        newPos.x = floor(collidedWith.right() - boundingBox.x);
        respnse.left = true;
      }
      velocity->v.x = 0.0f;
    }
  }

  position->x = newPos.x;
  position->y = newPos.y;

  return respnse;
}
