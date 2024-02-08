#include "abstractGameobject.h"

void debugRect(SDL_Rect r) {
  printf("(x: %d\ty: %d\tw: %d\th: %d)\n", r.x, r.y, r.w, r.h);
}

void AbstractGameObject::init(SDL_Renderer* renderer) {
  _collidable = collidable(_position, _renderable.textureRect);
}


void AbstractGameObject::update(float dt) {
  _collidable.update(_position);

  auto &v = _velocity;
  auto &p = _position;

  // Friction
  if (v.x > 0.2) {
    v.x -= v.friction;
  } else if (v.x < -0.2) {
    v.x += (v.friction * dt) / 10;
  } else {
    v.x = 0;
  }

  // Handle Y friction if we don't have gravity
  if (_gravity.entityGravity == 0.0f) {
    if (v.y > 0.2) {
      v.y -= v.friction;
    } else if (v.y < -0.2) {
      v.y += (v.friction * dt) / 10;
    } else {
      v.y = 0;
    }
  }
}

void AbstractGameObject::draw(SDL_Renderer* renderer, v2 offset) {
  // @TODO: add vectors instead
  _renderable.render(renderer, this->_position - offset);
}

void AbstractGameObject::setListenForCollisions() {
  _listenForCollisions = true;
}

bool AbstractGameObject::contains(SDL_Rect other) {
  return SDL_HasIntersection(&_collidable.rect, &other);
}

bool AbstractGameObject::contains(AbstractGameObject other) {
  return SDL_HasIntersection(&_collidable.rect, &other._collidable.rect);
}

SDL_Rect AbstractGameObject::getRect() {
  return {
    (int)_position.x,
    (int)_position.y,
    _collidable.rect.w,
    _collidable.rect.h,
  };
}

SDL_Rect* AbstractGameObject::getRectPointer() {
  return &this->_collidable.rect;
}

v2 AbstractGameObject::getPosition() {
  return this->_position;
}

v2* AbstractGameObject::getPositionPointer() {
  return &this->_position;
}

bool AbstractGameObject::getListensForCollisions() {
  return this->_listenForCollisions;
}

GAME_OBJECT AbstractGameObject::getType() {
  return _type;
}

OBJECT_TAG AbstractGameObject::getTag() {
  return _tag;
}
