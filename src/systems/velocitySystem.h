#pragma once
#include "SDL.h"
#include "../lib/entt/entt.hpp"
#include "components/position.h"
#include "components/velocity.h"

void velocitySystem(float dt, entt::registry &registry) {
  auto view = registry.view<position, velocity>();

  for (auto entity : view) {
    auto &vel = view.get<velocity>(entity);
    auto &pos = view.get<position>(entity);

    pos.x += vel.dx;
    pos.y += vel.dy;

    vel.dx = 0;
    vel.dy = 0;
  }
}

