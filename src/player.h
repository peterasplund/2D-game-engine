#pragma once
#include <SDL2/SDL.h>
#include "physicsObject.h"
#include "assetManager.h"
#include "object.h"
#include "animation.h"
#include "animator.h"
#include "inputHandler.h"
#include "camera.h"

enum State {
  S_IDLE,
  S_RUN,
  S_JUMP,
  S_ATTACK,
};

class Player : public PhysicsObject
{
private:
  Animator animator;
  InputHandler* _inputHandler;

  SDL_Rect leftRect;
  SDL_Rect rightRect;
  SDL_Rect topRect;
  SDL_Rect bottomRect;

  float _jumpPower = 3.0f;
  float backDashSpeed = 1.5f;
  bool isBackDashing = false;
  float runSpeed = 1.1f;
  std::string direction = "right";
  State _state = State::S_IDLE;

  Timer _timer;
public:

  Player(SDL_Renderer* renderer) : PhysicsObject() {
    size = { 48.0f, 48.0f };
    hitBox = { (int)14.0f, (int)14.0f, (int)22.0f, (int)34.0f };
    speed = 1.0;
    velocity = { 0, 0 };
    id = "player";

    _timer = Timer();
    _inputHandler = InputHandler::Instance();
    SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/LightBandit_Spritesheet.png");

    int tw = 48;
    int th = 48;

    Animation* animIdle = new Animation(texture);
    Animation* animRun = new Animation(texture);
    Animation* animAttack = new Animation(texture, false);
    Animation* animJump = new Animation(texture, false);
    Animation* animFall = new Animation(texture, false);
    Animation* animBackDash = new Animation(texture, false);

    animIdle->addFrame({ tw * 0, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 1, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 2, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 3, th * 0, tw, th }, 500);

    animRun->addFrame({ tw * 0, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 1, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 2, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 3, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 4, th * 1, tw, th }, 100);

    animAttack->addFrame({ tw * 3, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 4, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 5, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 6, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 7, th * 2, tw, th }, 100);

    animJump->addFrame({ tw * 5, th * 4, tw, th }, 100);

    animFall->addFrame({ tw * 5, th * 4, tw, th }, 100);

    animBackDash->addFrame({ tw * 1, th * 4, tw, th }, 100);


    animator.addAnimation("idle", animIdle);
    animator.addAnimation("run", animRun);
    animator.addAnimation("attack", animAttack);
    animator.addAnimation("jump", animJump);
    animator.addAnimation("fall", animFall);
    animator.addAnimation("backDash", animBackDash);

    animator.setAnimation("fall");

    animator.start();
  }

  void onCollisionTop() {

  }

  void onCollisionBottom() {

  }

  void onCollisionLeft() {

  }

  void onCollisionRight() {

  }

  void update(float dt, std::vector<Object> tiles) {
    if (_state != State::S_ATTACK) {
      if (_inputHandler->isPressed(BUTTON::LEFT)) {
        direction = "left";
        velocity.x = -runSpeed;
        textureFlip = SDL_FLIP_NONE;
      }

      if (_inputHandler->isPressed(BUTTON::RIGHT)) {
        direction = "right";
        velocity.x = runSpeed;
        textureFlip = SDL_FLIP_HORIZONTAL;
      }

      if (_inputHandler->isPressed(BUTTON::JUMP) && onFloor) {
        velocity.y = -_jumpPower;
      }
    }

    if (_inputHandler->isPressed(BUTTON::ATTACK) && onFloor && _state == State::S_IDLE) {
      _state = State::S_ATTACK;
      animator.setAnimation("attack");
      animator.reset();
      _timer.reset();
    }

    // back dash after attack
    if (_state == State::S_ATTACK && _timer.elapsed() > 300) {
      if (direction == "right" && _inputHandler->isPressed(BUTTON::LEFT)) {
        isBackDashing = true;
        velocity.x = -backDashSpeed;
        animator.setAnimation("backDash");
        animator.reset();
      } else if (direction == "left" && _inputHandler->isPressed(BUTTON::RIGHT)) {
        isBackDashing = true;
        velocity.x = backDashSpeed;
        animator.setAnimation("backDash");
        animator.reset();
      }
    }

    // 500 = 100ms on each frame of attack animation
    if (_state == State::S_ATTACK && _timer.elapsed() > 500) {
      isBackDashing = false;
      _state = State::S_IDLE;
    }

    if (_state != State::S_ATTACK) {
      if (velocity.y > .4f && !onFloor) {
        if (animator.getCurrent() != "fall") {
          animator.reset();
          animator.setAnimation("fall");
        }
      } else if (velocity.y < 0 && !onFloor) {
        if (animator.getCurrent() != "jump") {
          animator.reset();
          animator.setAnimation("jump");
        }
      } else if (velocity.x != 0 && onFloor) {
        if (animator.getCurrent() != "run") {
          animator.setAnimation("run");
        }
      } else if (onFloor) {
        if (animator.getCurrent() != "idle") {
          animator.setAnimation("idle");
        }
      }
    }

    if (position.y > 370) {
      // die and respawn
      position.y = -size.y;
    }

    PhysicsObject::update(dt);
    PhysicsObject::resolveCollisions(tiles);
  }

  virtual void draw(SDL_Renderer* renderer, SDL_Rect origin) {
    textureRect = animator.getFrame();
    setTexture(animator.getTexture());

    Object::draw(renderer, origin);
    //PhysicsObject::drawCollisionRectangles(renderer, origin);
  }
};
