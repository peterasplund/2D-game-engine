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
      virtual ~Player() { }
      void init() override;
      void update(float dt) override;
      void performJump();
      void jump();
      void attack();
      void onInputPressed(int button) override;
      void onInputReleased(int button) override;
      void draw(SDL_Renderer* renderer, v2f offset) override;

    protected:
      /// How high the initial jump should be
      const float JUMP_POWER = 0.4f;
      /// How much higher the player should go if they hold the jump button (lower value means higher jump).
      float JUMP_SHORT_GRAVITY = 0.006f;
      /// How fast to get to max run speed
      const float RUN_ACCELERATION = 0.015f;
      /// How fast to get to a stop
      const float RUN_DEACCELERATION = 0.02f;
      /// Max run speed
      const float RUN_SPEED = 0.2f;
      /// Jump buffer window in MS
      const int JUMP_BUFFER_WINDOW = 100; 
      // Error margin to land on one-way platform. How many pixels below we can be and still snap on top of it.
      const int ONEWAY_PLATFORM_GRACE = 3;

      GAME_OBJECT _type = GAME_OBJECT::PLAYER;
      Animator _animator;
      std::string direction = "right";
      State state = State::IDLE;
      Timer attackTimer;
      Timer slideTimer;
      Timer hurtTimer;
      Timer jumpBufferTimer;
      float _normalGravity;
      bool _jumpHold = false;
      bool _justJumped = false;
      float backDashSpeed = 1.5f;
      float attackSpeed = 3.0f;
      float attackDelay = 450.0f;
      float slideDelay = 200.0f;
      bool isBackDashing = false;
      bool jumpBuffered = false;
      bool previouslyOnFloor = false;
  };
}
