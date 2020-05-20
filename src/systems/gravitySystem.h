#pragma once
#include "../stdafx.h"
#include "../components/gravity.h"
#include "../components/position.h"
#include "../components/velocity.h"

void gravitySystem(float dt) {
  auto view = registry.view<gravity, position, velocity>();

  for (auto entity : view) {
    auto &v = view.get<velocity>(entity);
    auto &p = view.get<position>(entity);
    auto &g = view.get<gravity>(entity);

    if (v.y < 0.0f) {
      g.onFloor = false;
    }

    if (!g.onFloor) {
      v.y += (g.entityGravity * dt) / 10;
    }

    if (v.y > g.maxFallSpeed) {
      v.y = g.maxFallSpeed;
    }

  }
}
