#include "collidable.h"
#include "../debugPrinter.h"

struct CollisionInfo {
  int layer;
  int index;
  float t_hit;
  v2f contactNormal;
  Rect rect;
};

// Calculate "Near time" and "Far time"
// https://youtu.be/8JJ-4JgR7Dg?t=1813
//
// @TODO: I think this function is faulty. We can move through stuff on the up direction
// All values coming in to this function seem fine
bool rayVsRect(v2f& ray_origin, v2f& ray_dir, Rect* target,
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

bool DynamicRectVsRect(
    RectF* r_dynamic, v2f inVelocity,
    Rect& r_static, v2f& contact_point,
    v2f& contact_normal, float& contact_time, float dt) {
  if (inVelocity.x == 0.0f && inVelocity.y == 0.0f) {
    return false;
  }

  Rect expanded_target;
  expanded_target.setPos(r_static.pos() - r_dynamic->size() / 2);
  expanded_target.setSize(r_static.size() + r_dynamic->size());

  v2f ray_origin = r_dynamic->pos() + r_dynamic->size() / 2;

  v2f velocity = inVelocity * dt;

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

bool ResolveDynamicRectVsRect(RectF* inRect, v2f *inVelocity, float fTimeStep, Rect* r_static)
{
  v2f contact_point;
  v2f contact_normal;
  float contact_time = 0.0f;

  //Rect inRect, v2 inVelocity, Rect& target, v2& contact_point, v2i& contact_normal, float& contact_time, float dt
  if (DynamicRectVsRect(inRect, *inVelocity, *r_static, contact_point, contact_normal, contact_time, fTimeStep))
  {
    /*
    if (contact_normal.y > 0) r_dynamic->contact[0] = r_static; else nullptr;
    if (contact_normal.x < 0) r_dynamic->contact[1] = r_static; else nullptr;
    if (contact_normal.y < 0) r_dynamic->contact[2] = r_static; else nullptr;
    if (contact_normal.x > 0) r_dynamic->contact[3] = r_static; else nullptr;
    */

    v2f velocity = { inVelocity->x, inVelocity->y };
    velocity = contact_normal * v2f(std::abs(velocity.x), std::abs(velocity.y)) * (1 - contact_time);
    inVelocity->x += velocity.x;
    inVelocity->y += velocity.y;
    return true;
  }

  return false;
}


collidable::collidable() { }

collidable::collidable(v2f position, Rect boundingBox) {
  this->boundingBox = boundingBox;
}

/*
bool collidable::checkCollision(RectF* r, Tilemap* tilemap, Rect* outRect) {
  // Don't bother checking if the rect is completely outside the map
  if (r->y + r->h <= 0 || r->x + r->w <= 0) {
    outRect = nullptr;
    return false;
  }
  
  auto layers = tilemap->getLayers();

  for (int layer = 0; layer < layers->size(); layer++) {
    std::vector<int> possibleIndices = tilemap->getIndicesWithinRect(*r, layer);
    // For every tiles index within the collision rect
    for (int possibleIdx : possibleIndices) {
      // Skip indices outside of the map

      auto tiles = layers->at(layer).tiles;
    
      if (possibleIdx < tiles.size()) {
        int tileId = tiles.at(possibleIdx);

        if (tileId == 0) {
          continue;
        }

        //auto data = tilemap->getTileset()->getTileData(idx);
        Rect rect = tilemap->getTilePosition(layer, possibleIdx);
        if (!r->hasIntersection(&rect)) {
          continue;
        }

        // @TODO: get tile from tileId


        if (outRect != nullptr) {
          outRect->x = rect.x;
          outRect->y = rect.y;
          outRect->w = rect.w;
          outRect->h = rect.h;
          //printf("idx: %d\n", idx);
          //printf("tileId: %d\n", tileId);
          //outRect->debug();
        }

        //printf("end if\n");
        return true;
      }
    }
  }
  outRect = nullptr;
  return false;
}
*/

bool collidable::collideAt(v2f p, Rect* outRect) {
  return false;
  /*
  Tilemap* t = EntityManager::Instance()->getTilemap();
  if (t == nullptr) {
    return false;
  }

  RectF r = addBoundingBox(p);
  return checkCollision(&r, t, outRect);
  */
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

/// Moving with collision resolving
/*
CollisionResponse collidable::moveAndSlide(v2* position, velocity* velocity, float dt) {
  CollisionResponse respnse = { false, false, false, false };
  Rect collidedWith;
  v2 p = *position;

  if (velocity->y != 0) {
    int pixelsToMove = round((velocity->y / 10) * dt);
    int sign = pixelsToMove > 0 ? 1 : -1;
    pixelsToMove = abs(pixelsToMove);

    int i = 0;
    while (i < pixelsToMove) {
      float newY = p.y + ((i + 1) * sign);
      i += 1;
      if (collideAt({round(p.x), newY}, &collidedWith)) {
        if (newY > p.y) {
          position->y = floor(collidedWith.y - boundingBox.y - boundingBox.h) - 1;
          respnse.top = true;
        }
        else if (newY < p.y) {
          position->y = floor(collidedWith.bottom() - boundingBox.y);
          respnse.bottom = true;
        }
        velocity->y = 0;
        break;
      }
      else {
        position->y = newY;
      }
    }
  }

  p = *position;

  if (velocity->x != 0) {
    int pixelsToMove = round((velocity->x / 10) * dt);
    int sign = pixelsToMove > 0 ? 1 : -1;
    pixelsToMove = abs(pixelsToMove);

    int i = 0;
    while (i < pixelsToMove) {
      float newX = p.x + ((i + 1) * sign);
      i += 1;
      if (collideAt({newX, floor(p.y)}, &collidedWith)) {
        if (newX > p.x) {
          position->x = floor(collidedWith.x - boundingBox.x - boundingBox.w);
          respnse.left = true;
        }
        else if (newX < p.x) {
          position->x = floor(collidedWith.right() - boundingBox.x);
          respnse.right = true;
        }
        velocity->x = 0;
        break;
      }
      else {
        position->x = newX;
      }
    }
  }

  return respnse;
}
*/

void collidable::moveAndSlide2(v2f position, v2f* velocity, float dt) {
  RectF r = addBoundingBox(position);

  v2f contact_point;
  v2f contact_normal;
  float t_hit = 0;
  std::vector<CollisionInfo> collisions;

  Tilemap* tilemap = EntityManager::Instance()->getTilemap();
  auto layers = tilemap->getLayers();

  /*
  Rect pos = {
    (int)nextPositionF.x,
    (int)nextPositionF.y,
    (int)ceil(nextPositionF.w),
    (int)ceil(nextPositionF.h),
  };
  */

  RectF nextPositionF = addBoundingBox(v2f {
    round(position.x + velocity->x * dt),
    round(position.y + velocity->y * dt),
  });

  Rect nextPosition = Rect {
    (int)round(nextPositionF.x),
    (int)round(nextPositionF.y),
    (int)nextPositionF.w,
    (int)nextPositionF.h,
  };
  Rect summedPositions = group_rects(r, nextPositionF);

  for (int layer = 0; layer < layers->size(); layer++) {
    std::vector<int> possibleIndices = tilemap->getIndicesWithinRect(summedPositions, layer);
    for (int possibleIdx : possibleIndices) {
      int tileId = 0;
      tileId = layers->at(layer).tiles.at(possibleIdx);

      if (tileId == 0) {
        continue;
      }

      Rect otherRect = tilemap->getTilePosition(layer, possibleIdx);
      DebugPrinter::Instance()->addDebugRect(&otherRect, 255, 255, 0);

      bool result = DynamicRectVsRect(&r, *velocity, otherRect, contact_point, contact_normal, t_hit, dt);

      if (result) {
        collisions.push_back(CollisionInfo {
          possibleIdx,
          layer,
          t_hit,
          contact_normal,
          otherRect,
        });
      }
    }
  }

  std::sort(collisions.begin(), collisions.end(), [](CollisionInfo &a, CollisionInfo& b) {
    return a.t_hit < b.t_hit;
  });

  // printf("collisions: %d\n", collisions.size());
  for (auto j : collisions) {
    ResolveDynamicRectVsRect(&r, velocity, dt, &j.rect);
    /*
    v2f vel = { velocity->x, velocity->y };

    vel = j.contactNormal * v2f(std::abs(velocity->x), std::abs(velocity->y)) * (1 - j.t_hit);

    velocity->x += vel.x;
    velocity->y += vel.y;
    */
  }
}
