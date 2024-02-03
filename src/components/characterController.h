#pragma once
#include "../stdafx.h"
#include <vector>
#include <string>
#include <map>
#include "./hurtable.h"
#include "./solid.h"
#include "./animator.h"
#include "../inputHandler.h"
#include "./velocity.h"
#include "../timer.h"
#include "../events.h"
#include "../systems/animationSystem.h"

enum State {
  S_IDLE,
  S_RUN,
  S_JUMP,
  S_ATTACK,
  S_HURT,
  S_SLIDE,
};

struct characterController {
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

void hurtOnTouchHurtableBind() {
  struct collisionListener {
    void receive(const collisionEvent &e) {
      if (!registry.valid(e.self) || !registry.valid(e.other)) {
        return;
      }
      
      if (registry.has<hurtable>(e.other)) {
        auto &c = registry.get<characterController>(e.self);
        auto &v = registry.get<velocity>(e.self);
        auto &a = registry.get<animator>(e.self);


        if (c.state == S_HURT) {
          return;
        }

        printf("hurt\n");

        c.hurtTimer.reset();
        c.state = S_HURT;

        AnimationHelpers::reset(&a);
        AnimationHelpers::setAnimation(&a, "fall");
        
        if (c.direction == "left") {
          v.y = -4.0f;
          v.x = 6.0f;
        }

        if (c.direction == "right") {
          v.y = -4.0f;
          v.x = -6.0f;
        }

        return;
      }

      if (registry.has<solid>(e.other)) {
        auto c = registry.try_get<characterController>(e.self);

        if (c != nullptr && c->state == S_HURT) {
          c->state = S_IDLE;
        }
      }
    }
  };

  collisionListener listener;
  dispatcher.sink<collisionEvent>().connect<&collisionListener::receive>(listener);
}


