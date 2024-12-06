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

collidable::collidable() {}

collidable::collidable(v2f position, Rect boundingBox) {
  this->boundingBox = boundingBox;
  this->rect = {position.x + boundingBox.x, position.y + boundingBox.y,
                (float)boundingBox.w, (float)boundingBox.h};
}

// Calculate "Near time" and "Far time"
// https://youtu.be/8JJ-4JgR7Dg?t=1813
bool rayVsRect(v2f &ray_origin, v2f &ray_dir, RectF *target, v2f &contact_point,
               v2f &contact_normal, float &t_hit_near) {
  contact_normal = {0, 0};
  contact_point = {0, 0};

  // Cache division
  v2f invdir = 1.0f / ray_dir;
  v2i targetPos = target->pos();
  v2i targetSize = target->size();

  // Calculate intersections with rectangle bounding axes
  v2f t_near = (targetPos - ray_origin) * invdir;
  v2f t_far = (targetPos + targetSize - ray_origin) * invdir;

  if (std::isnan(t_far.y) || std::isnan(t_far.x))
    return false;
  if (std::isnan(t_near.y) || std::isnan(t_near.x))
    return false;

  // Sort distances
  if (t_near.x > t_far.x)
    std::swap(t_near.x, t_far.x);
  if (t_near.y > t_far.y)
    std::swap(t_near.y, t_far.y);

  // Early rejection
  if (t_near.x > t_far.y || t_near.y > t_far.x)
    return false;

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
      contact_normal = {1, 0};
    } else {
      contact_normal = {-1, 0};
    }
  } else if (t_near.x < t_near.y) {
    if (invdir.y < 0) {
      contact_normal = {0, 1};
    } else {
      contact_normal = {0, -1};
    }
  }

  return true;
}

bool dynamicRectVsRect(RectF *r_dynamic, v2f inVelocity, Rect &r_static,
                       v2f &contact_point, v2f &contact_normal,
                       float &contact_time, double dt) {
  if (inVelocity.x == 0.0f && inVelocity.y == 0.0f) {
    return false;
  }

  RectF expanded_target;
  expanded_target.setPos(r_static.pos() - r_dynamic->size() / 2);
  expanded_target.setSize(r_static.size() + r_dynamic->size());

  v2f ray_origin = r_dynamic->pos() + r_dynamic->size() / 2;

  v2f velocity = inVelocity * dt;

  if (rayVsRect(ray_origin, velocity, &expanded_target, contact_point, contact_normal, contact_time)) {
    return (contact_time >= 0.0f && contact_time < 1.0f);
  }
  else {
    return false;
  }
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

std::vector<Rect> getCollisionAt(RectF r) {
  std::vector<Rect> rects;
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
        rects.push_back(otherRect);
        // return otherRect;
      }
    }
  }

  return rects;
}

CollisionResponse collidable::moveAndSlide(v2f *position, velocity *velocity,
                                           double dt) {
  CollisionResponse response;
  v2 newPos = *position + velocity->v * dt;
  std::vector<Rect> collidedWith = getCollisionAt(addBoundingBox({(float)newPos.x, (float)newPos.y}));

  v2f cp, cn;
  float ct = 0.0f;
  std::vector<v2i> normal;
  std::vector<std::pair<int, float>> z;

  RectF playerRect = {
    round(rect.x),
    round(rect.y),
    round(rect.w),
    round(rect.h),
  };

  for (int i = 0; i < collidedWith.size(); i++) {
    if (dynamicRectVsRect(&playerRect, velocity->v, collidedWith[i], cp, cn, ct, dt)) {
      z.push_back({i, ct});
      normal.push_back(cn);
      break;
    }
  }

  // Sort closest first
  std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
			{
				return a.second < b.second;
			});


  v2f newVelocity = velocity->v;

  for (int i = 0; i < z.size(); i ++) {
    if (dynamicRectVsRect(&playerRect, newVelocity, collidedWith[i], cp, cn, ct, dt)) {

      position->x += velocity->v.x * z[i].second; 
      position->y += velocity->v.y * z[i].second;

      // Slide
      //float remainingtime = 1.0f - z[i].second;

      //float remainingtime = z[i].second;
      //double dotprod = (velocity->v.x * normal[i].y + velocity->v.y * normal[i].x) * remainingtime; 


      float dotProduct = velocity->v.dotProduct(normal[i]);
      dotProduct = dotProduct * (float)z[i].second;
      newVelocity = dotProduct *  dotProduct * collisionNormal.x()));


      printf("vel: (%f, %f)\n", newVelocity.x, newVelocity.y);
      //printf("dotprod: %f\n", dotprod);

      //newVelocity.x = dotprod * normal[i].x; 
      //newVelocity.y = dotprod * normal[i].y; 
      break;
    }
  }

  velocity->v.x = newVelocity.x;
  velocity->v.y = newVelocity.y;

  position->x += round(velocity->v.x * dt);
  position->y += round(velocity->v.y * dt);

  return response;
}
