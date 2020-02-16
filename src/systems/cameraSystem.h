#pragma once

#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/camera.h"
#include "../components/renderable.h"

void cameraSystem(entt::registry* registry) {
  auto view = registry->view<camera>();

  for (auto entity : view) {
    auto &c = view.get<camera>(entity);

    // follow player with camera
    int px, py, pw, ph;
    auto playerView = registry->view<characterController, position, renderable>();
    for (auto entity : playerView) {
      position &playerPosition = playerView.get<position>(entity);
      renderable &playerRenderable = playerView.get<renderable>(entity);
      px = playerPosition.x;
      py = playerPosition.y;
      pw = playerRenderable.textureRect.w;
      ph = playerRenderable.textureRect.h;
    }

    c.pos.x = px + (pw / 2) - (c.viewport.x / 2);
    //c.pos.y = py + (ph / 2) - (c.viewport.y / 2);

    // clamp within bounds
    if (c.pos.x < c.bounds.x) { c.pos.x = c.bounds.x; }
    if (c.pos.x > c.bounds.w) { c.pos.x = c.bounds.w; }
    if (c.pos.y < c.bounds.y) { c.pos.y = c.bounds.y; }
    if (c.pos.y > c.bounds.h) { c.pos.x = c.bounds.h; }
  }
}
