#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/velocity.h"

void velocitySystem(float dt, entt::registry &registry) {
  auto view = registry.view<position, velocity>();

  for (auto entity : view) {
    auto &v = view.get<velocity>(entity);
    auto &p = view.get<position>(entity);

    if (v.x > 0.2) {
      v.x -= v.friction;
    } else if (v.x < -0.2) {
      v.x += (v.friction * dt) / 10;
    } else {
      v.x = 0;
    }

    p.x += (v.x / 10) * dt;
    p.y += (v.y / 10) * dt;
  }
}
