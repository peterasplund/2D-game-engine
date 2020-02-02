#pragma once
#include "SDL.h"
#include "../lib/entt/entt.hpp"
#include "components/renderable.h"
#include "components/position.h"

void renderableSystem(SDL_Renderer* renderer, entt::registry &registry) {
  auto view = registry.view<renderable, position>();

  for (auto entity : view) {
    auto &r = view.get<renderable>(entity);
    auto &p = view.get<position>(entity);

    SDL_Rect sr = r.textureRect;
    SDL_Rect dr = { (int)p.x, (int)p.y, (int)sr.w, (int)sr.h };

    SDL_RenderCopyEx(renderer, r.texture, &sr, &dr, 0, 0, r.textureFlip);
  }
}

