#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/animator.h"
#include "../components/skeleton.h"
#include "../components/velocity.h"
#include "../components/animator.h"
#include "../systems/animationSystem.h"

void skeletonAISystem( entt::registry* registry) {
  auto view = registry->view<skeleton, velocity, animator>();

  for (auto entity : view) {
    auto &s = view.get<skeleton>(entity);
    auto &v = view.get<velocity>(entity);
    auto &a = view.get<animator>(entity);

    switch (s.actions[s.state])
    {
    case skeleton::WALK_FORWARD:
      AnimationHelpers::setAnimation(&a, "walk");
      v.x = -s.speed;
      /* code */
      break;

    case skeleton::JUMP:
      AnimationHelpers::setAnimation(&a, "jump");
      v.y = -s.jumpHeight;

      s.incrementState();
      break;
    case skeleton::IDLE:
      AnimationHelpers::setAnimation(&a, "idle");
    
    default:
      break;
    }

    if (s.timer.elapsed() > 1000) {
      s.incrementState();
      s.timer.reset();
    }

  }
}

