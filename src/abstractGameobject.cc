#include "abstractGameobject.h"

void AbstractGameObject::init() {
  _collidable = collidable(_position, Rect::from_sdl_rect(_renderable.textureRect));
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

bool AbstractGameObject::contains(Rect other) {
  return _collidable.rect.hasIntersection(&other);
}

bool AbstractGameObject::contains(AbstractGameObject other) {
  return _collidable.rect.hasIntersection(&other._collidable.rect);
}

Rect AbstractGameObject::getRect() {
  return {
    (int)_position.x,
    (int)_position.y,
    _collidable.rect.w,
    _collidable.rect.h,
  };
}

Rect* AbstractGameObject::getRectPointer() {
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
