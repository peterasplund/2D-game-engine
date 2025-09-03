#include "collidable.h"
#include "../abstractGameobject.h"
// #include "../debugPrinter.h"

struct CollisionInfo {
  int layer;
  int index;
  int tileId;
  float t_hit;
  v2f contactNormal;
  Rect rect;
};

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

bool collidable::dynamicRectVsRect(
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

CollisionAt* getCollisionAt(RectF r) {
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
      // DebugPrinter::Instance()->addDebugRect(&otherRect, 255, 255, 0);

      if (SDL_HasIntersection(&rSDL, &otherRectSDL)) {
        CollisionAt* resp = new CollisionAt{possibleIdx, tile};
        return resp;
      }
    }
  }

  return nullptr;
}

CollisionResponse collidable::moveAndSlide(v2f *position, velocity *velocity,
                                           double dt) {
  Level *tilemap = EntityManager::Instance()->getTilemap();
  CollisionResponse response;
  v2f newPos = *position;
  CollisionAt* collidedWith;

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

    if (collidedWith != nullptr) {
      Rect otherRect = tilemap->getTileRect(collidedWith->idx);
      if (otherRect.w != -1 && otherRect.h != -1) {
        if (velocity->v.x > 0.0f) {
          newPos.x = floor(otherRect.x - boundingBox.x - boundingBox.w);
          response.right = collidedWith;
        } else {
          newPos.x = floor(otherRect.right() - boundingBox.x);
          response.left = collidedWith;
        }
        //velocity->v.x = 0;
        break;
      }
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

    newPos.y = velocity->v.y > 0 ? position->y + framePos : position->y - framePos;
    RectF r = addBoundingBox(newPos);
    collidedWith = getCollisionAt(r);
    if (collidedWith != nullptr) {
      Rect otherRect = tilemap->getTileRect(collidedWith->idx);

      if (otherRect.w != -1 && otherRect.h != -1) {
        if (velocity->v.y > 0.0f) {
          newPos.y = floor(otherRect.y - boundingBox.y - boundingBox.h);
          response.bottom = collidedWith;
        } else {
          newPos.y = floor(otherRect.bottom() - boundingBox.y);
          newPos.y = floor(otherRect.bottom() - boundingBox.y);
          response.top = collidedWith;
        }
        velocity->v.y = 0;
        break;
      }
    }
  }

  position->x = newPos.x;
  position->y = newPos.y;

  return response;
}
