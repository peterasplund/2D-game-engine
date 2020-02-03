#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/velocity.h"

void velocitySystem(float dt, entt::registry &registry) {
  auto view = registry.view<position, velocity>();

  for (auto entity : view) {
    auto &vel = view.get<velocity>(entity);
    auto &pos = view.get<position>(entity);

    pos.x += (vel.dx * dt / 40) * vel.speed;
    pos.y += (vel.dy * dt / 40) * vel.speed;

    vel.dx = 0;
    vel.dy = 0;
  }
}

