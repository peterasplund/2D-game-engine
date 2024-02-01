#pragma once
#include "../stdafx.h"
#include "../components/position.h"
#include "../components/renderable.h"
#include "../components/velocity.h"
#include "../components/characterController.h"
#include "../components/renderable.h"
#include "../components/animator.h"
#include "../components/gravity.h"
#include "../components/collidable.h"
#include "../objects/knife.h"

void characterControllerSystem(InputHandler* inputHandler, SDL_Renderer* renderer) {
  auto view = registry.view<characterController, position, velocity, renderable, collidable, animator, gravity>();

  for (auto entity : view) {
    auto &p = view.get<position>(entity);
    auto &v = view.get<velocity>(entity);
    auto &c = view.get<characterController>(entity);
    auto &r = view.get<renderable>(entity);
    auto &a = view.get<animator>(entity);
    auto &g = view.get<gravity>(entity);
    auto &co = view.get<collidable>(entity);
    // printf("onFloor: %d\n", g.onFloor);

    // die and respawn when falling of level
    if (p.y > 370.0f) {
      p.y = -(float)r.textureRect.h;
    }

    if (c.state == S_HURT && c.hurtTimer.elapsed() > 700) {
      c.state = S_IDLE;
    }

    if (c.state == S_HURT) {
      if (a._currentAnimation != "backDash") {
        AnimationHelpers::reset(&a);
        AnimationHelpers::setAnimation(&a, "backDash");
      }
      continue;
    }

    if (c.state == S_SLIDE) {
      if (a._currentAnimation != "slide") {
        AnimationHelpers::reset(&a);
        AnimationHelpers::setAnimation(&a, "slide");
        v.x = c.direction == "left" ? -6 : 6;
      }
      continue;
    }

    if (c.state != S_ATTACK) {
      if (inputHandler->isHeld(BUTTON::JUMP) && g.onFloor) {
        v.y = -c.jumpPower;
      }

      if (inputHandler->isHeld(BUTTON::LEFT)) {
        c.direction = "left";
        v.x = -c.runSpeed;
        r.textureFlip = SDL_FLIP_NONE;
        c.state = S_RUN;
      } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
        c.direction = "right";
        v.x = c.runSpeed;
        r.textureFlip = SDL_FLIP_HORIZONTAL;
        c.state = S_RUN;
      } else {
        c.state = S_IDLE;
      }
    }

    // End ttack
    if (c.attackTimer.elapsed() > c.attackDelay && c.state == S_ATTACK) {
      c.state = S_IDLE;
      AnimationHelpers::reset(&a);
      AnimationHelpers::setAnimation(&a, "idle");
    }

    // Attack
    if (inputHandler->isHeld(BUTTON::ATTACK) && c.attackTimer.elapsed() > c.attackDelay && g.onFloor) {
      c.state = S_ATTACK;
      AnimationHelpers::reset(&a);
      AnimationHelpers::setAnimation(&a, "attack");
      c.attackTimer.reset();
    }

    if (inputHandler->isHeld(BUTTON::DOWN) && c.attackTimer.elapsed() > c.attackDelay && g.onFloor) {
      c.state = S_SLIDE;
      c.attackTimer.reset();
    }

      /*
    if (inputHandler->isHeld(BUTTON::ATTACK) && c.attackTimer.elapsed() > c.attackDelay) {
      createKnife(renderer, {
        p.x + co.rect.w / 2 + 10.0f,
        p.y + co.rect.h / 2
        }, {
          c.direction == "right" ?
            c.attackSpeed :
            -c.attackSpeed,
          0.0f
        });
      c.attackTimer.reset();
    }
    */

    if (c.state != S_ATTACK) {
      if (v.x == 0.0f) {
        AnimationHelpers::setAnimation(&a, "idle");
      }

      if (v.y > .4f && !g.onFloor) {
        if (a._currentAnimation != "fall") {
          AnimationHelpers::reset(&a);
          AnimationHelpers::setAnimation(&a, "fall");
        }
      } else if (v.y < 0 && !g.onFloor) {
        if (a._currentAnimation != "jump") {
          AnimationHelpers::reset(&a);
          AnimationHelpers::setAnimation(&a, "jump");
        }
      } else if (c.state == S_RUN && g.onFloor) {
        if (a._currentAnimation != "run") {
          AnimationHelpers::setAnimation(&a, "run");
        }
      } else if (g.onFloor) {
        if (a._currentAnimation != "idle") {
          AnimationHelpers::setAnimation(&a, "idle");
        }
      }
    }
  }
}

