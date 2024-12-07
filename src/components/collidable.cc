#include "collidable.h"
#include "../debugPrinter.h"

struct CollisionInfo {
  int layer;
  int index;
  int tileId;
  float t_hit;
  v2f contactNormal;
  RectF rect;
};

collidable::collidable() {}

collidable::collidable(v2f position, Rect boundingBox) {
  this->boundingBox = boundingBox;
  this->rect = {position.x + boundingBox.x, position.y + boundingBox.y,
                (float)boundingBox.w, (float)boundingBox.h};
}

bool rayVsRect(const v2f& ray_origin, const v2f& ray_dir, RectF* target, v2f& contact_point, v2f& contact_normal, float& t_hit_near)
{
  contact_normal = { 0,0 };
  contact_point = { 0,0 };

  // Cache division
  v2f invdir = 1.0f / ray_dir;

  // Calculate intersections with rectangle bounding axes
  v2f t_near = (target->pos() - ray_origin) * invdir;
  v2f t_far = (target->pos() + target->size() - ray_origin) * invdir;

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

  if (t_near.x > t_near.y)
    if (invdir.x < 0)
      contact_normal = { 1, 0 };
    else
      contact_normal = { -1, 0 };
  else if (t_near.x < t_near.y)
    if (invdir.y < 0)
      contact_normal = { 0, 1 };
    else
      contact_normal = { 0, -1 };

  // Note if t_near == t_far, collision is principly in a diagonal
  // so pointless to resolve. By returning a CN={0,0} even though its
  // considered a hit, the resolver wont change anything.
  return true;
}

bool dynamicRectVsRect(RectF *r_dynamic, velocity* inVelocity, RectF &r_static,
                       v2f &contact_point, v2f &contact_normal,
                       float &contact_time, float dt) {
  if (inVelocity->v.x == 0.0f && inVelocity->v.y == 0.0f) {
    return false;
  }

  RectF expanded_target;
  expanded_target.setPos(r_static.pos() - r_dynamic->size() / 2);
  expanded_target.setSize(r_static.size() + r_dynamic->size());

  v2f ray_origin = r_dynamic->pos() + r_dynamic->size() / 2;

  v2f velocity = inVelocity->v * dt;

  if (rayVsRect(ray_origin, velocity, &expanded_target, contact_point,
                contact_normal, contact_time)) {
    return (contact_time >= 0.0f && contact_time < 1.0f);
  }

  return false;
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

std::vector<RectF> getCollisionAt(RectF r) {
  std::vector<RectF> rects;
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
        rects.push_back({ (float)otherRect.x, (float)otherRect.y, (float)otherRect.w, (float)otherRect.h });
        // return otherRect;
      }
    }
  }

  return rects;
}

CollisionResponse collidable::moveAndSlide(v2f *position, velocity *velocity, double dt) {

  CollisionResponse response;
  v2 newPos = *position + (velocity->v * dt);
  std::vector<RectF> collidedWith = getCollisionAt(addBoundingBox({(float)newPos.x, (float)newPos.y}));

  float ct = 0.0f;
  std::vector<v2i> normals;


  RectF playerR = {round(rect.x), round(rect.y), round(rect.w), round(rect.h)};
  //rect
	//RectF playerR = { rect.x, rect.y, rect.w, rect.h };

  v2f contactPoint;
  v2f contactNormal;

  std::vector<std::pair<int, float>> z;

  for (int i = 0; i < collidedWith.size(); i++) {
    bool result = dynamicRectVsRect(&playerR, velocity, collidedWith[i], contactPoint, contactNormal, ct, dt);
    if (result) {
      normals.push_back(contactNormal);
      z.push_back({i, ct});
    }
  }

	// Do the sort
  /*
	std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
		{
			return a.second < b.second;
		});
    */


  for (auto j : z) {
    bool result = dynamicRectVsRect(&playerR, velocity, collidedWith[j.first], contactPoint, contactNormal, ct, dt);
    if (result) {
      normals.push_back(contactNormal);
			velocity->v.x += contactNormal.x * std::abs(velocity->v.x) * (1 - ct);
    }
  }

	position->x += velocity->v.x * dt;

  for (auto j : z) {
    bool result = dynamicRectVsRect(&playerR, velocity, collidedWith[j.first], contactPoint, contactNormal, ct, dt);
    if (result) {
      normals.push_back(contactNormal);
			velocity->v.y += contactNormal.y * std::abs(velocity->v.y) * (1 - ct);
    }
  }

	position->y += velocity->v.y * dt;

	// Round on collision
	if (normals.size() > 0) {
		for(auto n : normals) {
			if (n.x != 0.0f) {
        LOG_WARN("Got collision X: %f", n.x);
				position->x = round(position->x);
			}
			if (n.y != 0.0f) {
        LOG_ERROR("Got collision Y: %f", n.y);
				position->y = round(position->y);
			}
		}
	}

  return response;
}
