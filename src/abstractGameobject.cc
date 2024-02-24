#include "abstractGameobject.h"

void AbstractGameObject::init() {
  _collidable = collidable(_position, Rect::from_sdl_rect(_renderable.textureRect));
}


void AbstractGameObject::update(float dt) {
  _collidable.update(_position);

  auto &p = _position;
  _velocity.v.x = 0;
  _velocity.v.y = 0;

  // Friction
  if (_velocity.v.x > 0.05) {
    _velocity.v.x -= _velocity.friction;
  } else if (_velocity.v.x < -0.05) {
    _velocity.v.x += _velocity.friction;
  } else {
    _velocity.v.x = 0;
  }
  /*

  // Handle Y friction if we don't have gravity
  //if (_gravity.entityGravity == 0.0f) {
    if (_velocity.v.y > 0.2) {
      _velocity.v.y -= _velocity.friction;
    } else if (_velocity.v.y < -0.2) {
      _velocity.v.y += (_velocity.friction * dt) / 10;
    } else {
      _velocity.v.y = 0;
    }
    */
  //}
}

void AbstractGameObject::draw(SDL_Renderer* renderer, v2f offset) {
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
  return false;
  //return _collidable.rect.hasIntersection(&other._collidable.rect);
}

Rect AbstractGameObject::getRect() {
  return {
    (int)_position.x,
    (int)_position.y,
    (int)_collidable.rect.w,
    (int)_collidable.rect.h,
  };
}

RectF AbstractGameObject::getRectFloat() {
  return {
    _position.x,
    _position.y,
    _collidable.rect.w,
    _collidable.rect.h,
  };
}

RectF* AbstractGameObject::getRectPointer() {
  return &this->_collidable.rect;
}

v2f AbstractGameObject::getPosition() {
  return this->_position;
}

v2f* AbstractGameObject::getPositionPointer() {
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
