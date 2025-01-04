#pragma once

#include "../abstractGameobject.h"
#include "../engine/animator.h"
#include "../engine/assetManager.h"
#include "../engine/renderer.h"
#include "../components/autoPatrol.h"

namespace obj {
class Skeleton : public AbstractGameObject {
  enum State { WALK, RISING, HIDING, MELTING };
  std::string stateToStr(State s) {
    if (s == State::WALK) {
      return "walk";
    }
    else if (s == State::RISING) {
      return "rising";
    }
    else if (s == State::MELTING) {
      return "melting";
    }
    else if (s == State::HIDING) {
      return "hiding";
    }

    return "-";
  }

  AutoPatrol autoPatrol;
public:
  void init() override {
    AbstractGameObject::init();
    _type = GAME_OBJECT::ENEMY;

    this->_collidable.boundingBox = {
        10,
        14,
        30,
        39,
    };

    this->_position.y = this->_position.y - 2;

    int tw = 44;
    int th = 52;

    SDL_Texture *texture = AssetManager::Instance()->getTexture(
        "assets/sprites/enemies/skeleton.png");

    Animation *animWalk = new Animation(texture);
    animWalk->addFrame({tw * 0, th * 0, tw, th}, 150);
    animWalk->addFrame({tw * 1, th * 0, tw, th}, 150);
    animWalk->addFrame({tw * 2, th * 0, tw, th}, 150);
    animWalk->addFrame({tw * 3, th * 0, tw, th}, 150);
    animWalk->addFrame({tw * 4, th * 0, tw, th}, 150);
    animWalk->addFrame({tw * 5, th * 0, tw, th}, 150);
    animWalk->addFrame({tw * 6, th * 0, tw, th}, 150);
    animWalk->addFrame({tw * 7, th * 0, tw, th}, 150);

    Animation *animRise = new Animation(texture, false);
    animRise->addFrame({tw * 0, th * 1, tw, th}, 100);
    animRise->addFrame({tw * 1, th * 1, tw, th}, 100);
    animRise->addFrame({tw * 2, th * 1, tw, th}, 100);
    animRise->addFrame({tw * 3, th * 1, tw, th}, 100);
    animRise->addFrame({tw * 4, th * 1, tw, th}, 100);
    animRise->addFrame({tw * 5, th * 1, tw, th}, 100);

    Animation *animMelt = new Animation(texture, false);
    animMelt->addFrame({tw * 5, th * 1, tw, th}, 100);
    animMelt->addFrame({tw * 4, th * 1, tw, th}, 100);
    animMelt->addFrame({tw * 3, th * 1, tw, th}, 100);
    animMelt->addFrame({tw * 2, th * 1, tw, th}, 100);
    animMelt->addFrame({tw * 1, th * 1, tw, th}, 100);
    animMelt->addFrame({tw * 0, th * 1, tw, th}, 100);

    this->_animator = Animator();
    _animator.addAnimation("walk", animWalk);
    _animator.addAnimation("rise", animRise);
    _animator.addAnimation("melt", animMelt);

    this->_renderable.texture = texture;
    _direction = Direction::LEFT;

    setState(State::RISING);
  }

  void setState(State s) {
    if (s == State::RISING) {
      _animator.setAnimation("rise");
    }

    if (s == State::MELTING) {
      _animator.setAnimation("melt");
    }

    if (s == State::WALK) {
      _animator.setAnimation("walk");
    }

    _stateTimer.reset();
    _state = s;
  }

  void update(double dt) override {
    AbstractGameObject::update(dt);

    if (autoPatrol.shouldTurn(&_collidable, _direction, true)) {
      _direction = _direction == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
    }

    _collidable.moveAndSlide(&_position, &_velocity, dt);
    _velocity.v.x = 0.0f;
    invincible = false;
    active = true;
    switch (_state) {
      case State::WALK:
        if (_direction == Direction::RIGHT) {
          _velocity.v.x = SPEED;
        }
        else {
          _velocity.v.x = -SPEED;
        }

        if (_stateTimer.elapsed() > _walkingTime) {
          setState(State::MELTING);
        }
        break;
      case State::RISING:
        invincible = true;
        active = false;
        if (_stateTimer.elapsed() > _risingTime) {
          setState(State::WALK);
        }
        break;
      case State::MELTING:
        invincible = true;
        active = false;
        if (_stateTimer.elapsed() > _meltingTime) {
          setState(State::HIDING);
        }
        break;
      case State::HIDING:
        invincible = true;
        active = false;
        if (_stateTimer.elapsed() > _hidingTime) {
          setState(State::RISING);
        }
        break;
      default:
        
        break;
    }

    if (_timer.elapsed() > 350) {
      hurt = false;
    }
  }

  void draw(Renderer *renderer) override {
    _renderable.textureRect = _animator.getFrame();
    _renderable.texture = _animator.getTexture();

    if (_direction == Direction::LEFT) {
      _renderable.textureFlip = SDL_FLIP_NONE;
    }
    else {
      _renderable.textureFlip = SDL_FLIP_HORIZONTAL;
    }

    if (hurt) {
      hurtTimer += 0.1;
    }

    if (!hurt || (int)hurtTimer % 2) {
      if (_state != State::HIDING) {
        AbstractGameObject::draw(renderer);
      }
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
  float SPEED = 0.015f;

  int _walkingTime = 3400;
  int _risingTime = 500;
  int _meltingTime = 650;
  int _hidingTime = 4000;

  Animator _animator;
  Direction _direction;
  State _state;

  Timer _timer;
  Timer _stateTimer;
  int hp = 3000;
  int damagePoints = 15;
  float hurtTimer;
};
} // namespace obj
