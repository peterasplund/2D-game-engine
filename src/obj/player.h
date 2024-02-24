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
          //22, 31 
          //22, 32 // Debug with two tiles tall 
          16, 16 
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

        if (inputHandler->isHeld(BUTTON::UP)) {
          _velocity.v.y = -runSpeed;
        } else if (inputHandler->isHeld(BUTTON::DOWN)) {
          _velocity.v.y = runSpeed;
        }
        if (inputHandler->isHeld(BUTTON::LEFT)) {
          _velocity.v.x = -runSpeed;
        } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
          _velocity.v.x = runSpeed;
        }
        
        _collidable.moveAndSlide2(_position, &_velocity.v, dt);

        _position.x += _velocity.v.x * dt;
        _position.y += _velocity.v.y * dt;

        _position.x = round(_position.x);
        _position.y = round(_position.y);

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

      float jumpPower = 6.0f;
      float backDashSpeed = 1.5f;
      float attackSpeed = 3.0f;
      float attackDelay = 450.0f;
      float slideDelay = 200.0f;
      bool isBackDashing = false;
      float runSpeed = 0.2f;
      std::string direction = "right";
      State state = State::IDLE;
      Timer attackTimer;
      Timer slideTimer;
      Timer hurtTimer;

      //SDL_Rect hurtBox = { (int)14.0f, (int)15.0f, (int)22.0f, (int)31.0f };
      //SDL_Rect hurtBoxSliding = { (int)14.0f, (int)15 + 26, (int)22.0f, (int)5.0f };
  };
}
