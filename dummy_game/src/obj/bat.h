#pragma once

#include <core/animator.h>
#include <core/assetManager.h>
#include <core/renderer.h>

#include "../abstractGameobject.h"

namespace obj {
class Bat : public AbstractGameObject {
public:
  void init() override {
    AbstractGameObject::init();
    _type = GAME_OBJECT::ENEMY;

    this->_collidable.boundingBox = {
        0,
        0,
        16,
        16,
    };

    int tw = 16;
    int th = 16;

    SDL_Texture *texture =
        AssetManager::Instance()->getTexture("assets/sprites/bat.png");

    Animation *animIdle = new Animation(texture);
    // 13 frames
    animIdle->addFrame({tw * 0, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 1, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 2, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 3, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 4, th * 0, tw, th}, 100);

    this->_animator = Animator();
    _animator.addAnimation("idle", animIdle);
    _animator.setAnimation("idle");

    this->_renderable.texture = texture;
    _position.x = 255;
    _position.y = 198;

    _velocity.v.x = -0.02f;
  }

  void update(double dt) override {
    _position += _velocity.v * dt;
    _position.y = AMPLITUDE * cos(SPEED / 2 * elapsedTime) + 50.0f;
    AbstractGameObject::update(dt);
    elapsedTime += dt;

    if (_timer.elapsed() > 350) {
      hurt = false;
    }

    if (_velocity.v.x > -0.02f) {
      _velocity.v.x -= 0.02f;
    } else if (_velocity.v.x < 0.02f) {
      _velocity.v.x = -0.02f;
    } else {
    }
  }

  void draw(Renderer *renderer) override {
    _renderable.textureRect = _animator.getFrame();
    _renderable.texture = _animator.getTexture();
    _renderable.textureFlip = SDL_FLIP_HORIZONTAL;

    if (hurt) {
      _hurtFlashTimer += 0.1;
    }

    if (!hurt || (int)_hurtFlashTimer % 2) {
      AbstractGameObject::draw(renderer);
    }
  }

  virtual void damage(int dmg) override {
    if (!hurt) {
      hp -= dmg;
      hurt = true;
      _timer.reset();
    }
  }

protected:
  float SPEED = 0.005f;
  float AMPLITUDE = 15.0f;
  float elapsedTime = 0.0f;
  Animator _animator;

  Timer _timer;
  int hp = 3000;
  float _hurtFlashTimer;
};
} // namespace obj
