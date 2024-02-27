#include "abstractGameobject.h"

void AbstractGameObject::init() {
  _collidable = collidable(_position, Rect::from_sdl_rect(_renderable.textureRect));
}

void AbstractGameObject::update(float dt) {
  if (_gravity.entityGravity != 0.0f) {
    _gravity.update(&_position, &_velocity, dt);
  }

  _collidable.update(_position);
}

void AbstractGameObject::draw(SDL_Renderer* renderer, v2f offset) {
  // @TODO: add vectors instead
  _renderable.render(renderer, this->_position - offset);
}

void AbstractGameObject::setListenForCollisions() {
  _listenForCollisions = true;
}

bool AbstractGameObject::contains(RectF other) {
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
