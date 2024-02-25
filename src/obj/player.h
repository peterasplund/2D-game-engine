#pragma once

#include "../abstractGameobject.h"
#include "../animator.h"
#include "../assetManager.h"
#include "../inputHandler.h"

namespace obj {
  enum class State {
    IDLE,
    RUN,
    JUMP,
    ATTACK,
    HURT,
    SLIDE,
  };

  class Player : public AbstractGameObject {
    public:
      virtual ~Player() {
      }

      void init() override {
        AbstractGameObject::init();

        _tag = OBJECT_TAG::PLAYER;

        this->_collidable.boundingBox = { 
          14, 15, 
          22, 31 
        };

        setListenForCollisions();

        SDL_Texture* texture = AssetManager::Instance()->getTexture("assets/sprites/LightBandit_Spritesheet.png");

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
        
        _animator.setAnimation("idle");

        this->_renderable.texture = texture;
      }

      void update(float dt) override {
        AbstractGameObject::update(dt);
        InputHandler* inputHandler = InputHandler::Instance();

      if (state == State::SLIDE) {
        if (_animator.getCurrent() != "slide") {
          _animator.reset();
          _animator.setAnimation("slide");
          _velocity.v.x += direction == "left" ? -6 : 6;
        }
      }

      if (state != State::ATTACK) {
        if (state != State::JUMP && inputHandler->isHeld(BUTTON::JUMP) && (_gravity.onFloor || state == State::SLIDE)) {
          state = State::JUMP;
          _velocity.v.y = -jumpPower;
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

        if (state != State::SLIDE) {
          if (inputHandler->isHeld(BUTTON::LEFT)) {
            direction = "left";
            _velocity.v.x = -runSpeed;
            _renderable.textureFlip = SDL_FLIP_NONE;
            state = State::RUN;
          } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
            direction = "right";
            _velocity.v.x = runSpeed;
            _renderable.textureFlip = SDL_FLIP_HORIZONTAL;
            state = State::RUN;
          } else if (state != State::SLIDE) {
            state = State::IDLE;
          }
        }
      }

      if (state == State::ATTACK) {
          _animator.setAnimation("attack");
      }
      else if (!_gravity.onFloor) {
          _animator.setAnimation("fall");
      }
      else {
        if (state == State::RUN) {
          _animator.setAnimation("run");
        }
        else {
          _animator.setAnimation("idle");
        }
      }

      // End ttack
      if (attackTimer.elapsed() > attackDelay && state == State::ATTACK) {
        state = State::IDLE;
        _animator.reset();
        _animator.setAnimation("idle");
      }

      // End slide
      /*
      if (slideTimer.elapsed() > slideDelay && state == State::SLIDE) {
        state = State::IDLE;
        _animator.reset();
        _animator.setAnimation("idle");
      }
      */

      // Attack
      if (inputHandler->isHeld(BUTTON::ATTACK) && attackTimer.elapsed() > attackDelay/* && g.onFloor*/) {
        state = State::ATTACK;
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

        auto resp = _collidable.moveAndSlide(_position, &_velocity.v, dt);
        
        if (resp.hasCollision()) {
          resp.print();
        }

        _position.x += _velocity.v.x * dt;
        _position.y += _velocity.v.y * dt;


        auto collisionBelow = _collidable.tileExistsAt({
          (int)_collidable.rect.x,
          (int)(_collidable.rect.y + _collidable.rect.h + 1),
          (int)_collidable.rect.w,
          1
        });

        _gravity.onFloor = false;

        if (collisionBelow.size() > 0) {
          for(auto collision : collisionBelow) {
            TileData tileData = EntityManager::Instance()->getTilemap()->getTileData(collision.tileId);

            if (tileData.solid) {
              _gravity.onFloor = true;
            }
          }
        }


        auto &v = _velocity;
        auto &p = _position;
      }

      void draw(SDL_Renderer* renderer, v2f offset) override {
        _renderable.textureRect = _animator.getFrame();
        _renderable.texture = _animator.getTexture();

        AbstractGameObject::draw(renderer, offset);
      }

    protected:
      GAME_OBJECT _type = GAME_OBJECT::PLAYER;
      Animator _animator;

      float jumpPower = 0.7f;
      float backDashSpeed = 1.5f;
      float attackSpeed = 3.0f;
      float attackDelay = 450.0f;
      float slideDelay = 200.0f;
      bool isBackDashing = false;
      float runSpeed = 0.25f;
      std::string direction = "right";
      State state = State::IDLE;
      Timer attackTimer;
      Timer slideTimer;
      Timer hurtTimer;

      //SDL_Rect hurtBox = { (int)14.0f, (int)15.0f, (int)22.0f, (int)31.0f };
      //SDL_Rect hurtBoxSliding = { (int)14.0f, (int)15 + 26, (int)22.0f, (int)5.0f };
  };
}
