#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/renderable.h"
#include "../components/velocity.h"
#include "../components/characterController.h"
#include "../components/renderable.h"
#include "../components/animator.h"

void characterControllerSystem(InputHandler* inputHandler, entt::registry &registry) {
  auto view = registry.view<characterController, velocity, renderable, animator>();

  for (auto entity : view) {
    auto &v = view.get<velocity>(entity);
    auto &c = view.get<characterController>(entity);
    auto &r = view.get<renderable>(entity);
    auto &a = view.get<animator>(entity);

    if (inputHandler->isHeld(BUTTON::LEFT)) {
      c.direction = "left";
      v.dx = -c.runSpeed;
      r.textureFlip = SDL_FLIP_NONE;
      AnimationHelpers::setAnimation(&a, "run");
    }

    if (inputHandler->isHeld(BUTTON::RIGHT)) {
      c.direction = "right";
      v.dx = c.runSpeed;
      r.textureFlip = SDL_FLIP_HORIZONTAL;
      AnimationHelpers::setAnimation(&a, "run");
    }

    if (v.dx == 0.0f) {
      AnimationHelpers::setAnimation(&a, "idle");
    }
  }
}

