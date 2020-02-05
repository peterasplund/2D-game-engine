#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/renderable.h"
#include "../components/velocity.h"
#include "../components/characterController.h"
#include "../components/renderable.h"
#include "../components/animator.h"
#include "../components/gravity.h"

void characterControllerSystem(InputHandler* inputHandler, entt::registry &registry) {
  auto view = registry.view<characterController, position, velocity, renderable, animator, gravity>();

  for (auto entity : view) {
    auto &p = view.get<position>(entity);
    auto &v = view.get<velocity>(entity);
    auto &c = view.get<characterController>(entity);
    auto &r = view.get<renderable>(entity);
    auto &a = view.get<animator>(entity);
    auto &g = view.get<gravity>(entity);
    printf("onFloor: %d\n", g.onFloor);

    if (inputHandler->isHeld(BUTTON::JUMP) && g.onFloor) {
      v.y = -c.jumpPower;
    }

    if (inputHandler->isHeld(BUTTON::LEFT)) {
      c.direction = "left";
      v.x = -c.runSpeed;
      r.textureFlip = SDL_FLIP_NONE;
    }

    if (inputHandler->isHeld(BUTTON::RIGHT)) {
      c.direction = "right";
      v.x = c.runSpeed;
      r.textureFlip = SDL_FLIP_HORIZONTAL;
    }

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
    } else if (v.x != 0 && g.onFloor) {
      if (a._currentAnimation != "run") {
        AnimationHelpers::setAnimation(&a, "run");
      }
    } else if (g.onFloor) {
      if (a._currentAnimation != "idle") {
        AnimationHelpers::setAnimation(&a, "idle");
      }
    }




    if (p.y > 370.0f) {
      // die and respawn
      p.y = -(float)r.textureRect.h;
    }
  }
}

