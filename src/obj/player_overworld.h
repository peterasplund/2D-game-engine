#pragma once

#include "../abstractGameobject.h"
#include "../engine/animator.h"
#include "../engine/assetManager.h"
#include "../engine/inputHandler.h"
#include "../engine/renderer.h"

namespace obj {
class PlayerOverworld : public AbstractGameObject {
  enum class State {
    IDLE,
    WALK,
  };

public:
  virtual ~PlayerOverworld() {}

  void init() override {
    AbstractGameObject::init();

    _tag = OBJECT_TAG::PLAYER;

    this->_collidable.boundingBox = {0, 0, 16, 16};

    setListenForCollisions();

    SDL_Texture *texture = AssetManager::Instance()->getTexture(
        "assets/sprites/player_overworld.png");

    int tw = 16;
    int th = 16;

    _renderable.spriteOffset = {0, 0};
    _renderable.textureRect = {0, 0, 16, 16};

    this->_renderable.texture = texture;

    _gravity.entityGravity = 0.0f;
  }

  void update(float dt) override {
    AbstractGameObject::update(dt);
    InputHandler *inputHandler = InputHandler::Instance();

    if (inputHandler->isHeld(BUTTON::UP)) {
      direction = "up";
      _velocity.v.y = -walkSpeed;
    } else if (inputHandler->isHeld(BUTTON::DOWN)) {
      direction = "down";
      _velocity.v.y = walkSpeed;
    }

    if (inputHandler->isHeld(BUTTON::LEFT)) {
      direction = "left";
      _velocity.v.x = -walkSpeed;
    } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
      direction = "right";
      _velocity.v.x = walkSpeed;
    }

    _velocity.v.normalize();
    _velocity.v = _velocity.v / 7.0f;

    auto resp = _collidable.moveAndSlide(&_position, &_velocity, dt);

    _velocity.v.x = 0.0f;
    _velocity.v.y = 0.0f;
  }

  void draw(Renderer *renderer) override { AbstractGameObject::draw(renderer); }

protected:
  GAME_OBJECT _type = GAME_OBJECT::PLAYER;
  Animator _animator;

  float walkSpeed = 0.2f;
  std::string direction = "right";
  State state = State::IDLE;
};
} // namespace obj
