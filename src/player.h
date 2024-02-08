#pragma once

#include "abstractGameobject.h"
#include "animator.h"
#include "assetManager.h"
#include "inputHandler.h"
#include "components/animator.h"

enum State {
  S_IDLE,
  S_RUN,
  S_JUMP,
  S_ATTACK,
  S_HURT,
  S_SLIDE,
};

class Player : public AbstractGameObject {
  public:
    virtual ~Player() {
    }

    void init(SDL_Renderer* renderer) override {
      AbstractGameObject::init(renderer);

      this->_collidable.boundingBox = { 
        14, 15, 
        22, 31 
      };

      setListenForCollisions();

      SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/LightBandit_Spritesheet.png");

      int tw = 48;
      int th = 48;

      Animation* animIdle = new Animation(texture);
      Animation* animRun = new Animation(texture);
      Animation* animAttack = new Animation(texture, false);
      Animation* animJump = new Animation(texture, false);
      Animation* animFall = new Animation(texture, false);
      Animation* animBackDash = new Animation(texture, false);
      Animation* animSlide = new Animation(texture, false);

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

      animSlide->addFrame({ tw * 6, th * 4, tw, th }, 100);

      this->_animator = Animator();
      _animator.addAnimation("idle", animIdle);
      _animator.addAnimation("run", animRun);
      _animator.addAnimation("attack", animAttack);
      _animator.addAnimation("jump", animJump);
      _animator.addAnimation("fall", animFall);
      _animator.addAnimation("backDash", animBackDash);
      _animator.addAnimation("slide", animSlide);
      
      _animator.setAnimation("run");

      this->_renderable.texture = texture;
      this->_position = {128,128};
    }

    void update(float dt) override {
      InputHandler* inputHandler = InputHandler::Instance();
      _renderable.textureRect = _animator.getFrame();
      _renderable.texture = _animator.getTexture();

      if (state == S_SLIDE) {
        if (_animator.getCurrent() != "slide") {
          _animator.reset();
          _animator.setAnimation("slide");
          _velocity.x += direction == "left" ? -6 : 6;
        }
      }

      if (state != S_ATTACK) {
        if (state != S_JUMP && inputHandler->isHeld(BUTTON::JUMP) && (_gravity.onFloor || state == S_SLIDE)) {
          state = S_JUMP;
          _velocity.y = -jumpPower;
        }

        // temp move up/down
        /*
        if (inputHandler->isHeld(BUTTON::UP)) {
          _velocity.y = -runSpeed;
        }
        else if (inputHandler->isHeld(BUTTON::DOWN)) {
          _velocity.y = runSpeed;
        }
        */

        if (state != S_SLIDE) {
          if (inputHandler->isHeld(BUTTON::LEFT)) {
            direction = "left";
            _velocity.x = -runSpeed;
            _renderable.textureFlip = SDL_FLIP_NONE;
            state = S_RUN;
          } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
            direction = "right";
            _velocity.x = runSpeed;
            _renderable.textureFlip = SDL_FLIP_HORIZONTAL;
            state = S_RUN;
          } else if (state != S_SLIDE) {
            state = S_IDLE;
          }
        }
      }

      // End ttack
      if (attackTimer.elapsed() > attackDelay && state == S_ATTACK) {
        state = S_IDLE;
        _animator.reset();
        _animator.setAnimation("idle");
      }

      // End slide
      if (slideTimer.elapsed() > slideDelay && state == S_SLIDE) {
        state = S_IDLE;
        _animator.reset();
        _animator.setAnimation("idle");
      }

      // Attack
      if (inputHandler->isHeld(BUTTON::ATTACK) && attackTimer.elapsed() > attackDelay/* && g.onFloor*/) {
        state = S_ATTACK;
        _animator.reset();
        _animator.setAnimation("attack");
        attackTimer.reset();
      }

      // Slide
      /*
      if (inputHandler->isHeld(BUTTON::DOWN)*//* && g.onFloor*//*) {
        state = S_SLIDE;
        slideTimer.reset();
      }
      */

      // die and respawn when falling of level
      if (_position.y > 370.0f) {
        _position.y = -(float)_renderable.textureRect.h;
      }

      AbstractGameObject::update(dt);
    }

    void draw(SDL_Renderer* renderer, v2 offset) override {
      AbstractGameObject::draw(renderer, offset);
    }

    /*
    void onSolidCollision(SDL_Rect other, v2i cornerCollisions[4]) override {
      AbstractGameObject::onSolidCollision(other, cornerCollisions);

      // @TODO: run collision resolve function. 
      // That function will be placed somewhere neat.
      printf("Collision!\n");
    }
    */

  protected:
    GAME_OBJECT _type = GAME_OBJECT::PLAYER;
    Animator _animator;

    float jumpPower = 6.0f;
    float backDashSpeed = 1.5f;
    float attackSpeed = 3.0f;
    //float attackDelay = 5.0f;
    float attackDelay = 450.0f;
    float slideDelay = 200.0f;
    bool isBackDashing = false;
    float runSpeed = 1.4f;
    std::string direction = "right";
    State state = State::S_IDLE;
    Timer attackTimer;
    Timer slideTimer;
    Timer hurtTimer;

    SDL_Rect hurtBox = { (int)14.0f, (int)15.0f, (int)22.0f, (int)31.0f };
    SDL_Rect hurtBoxSliding = { (int)14.0f, (int)15 + 26, (int)22.0f, (int)5.0f };
};
