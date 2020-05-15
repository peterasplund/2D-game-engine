#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/renderable.h"
#include "../components/position.h"
#include "../components/collidable.h"
#include "../components/solid.h"
#include "../components/camera.h"

void debugSystem(SDL_Renderer* renderer, entt::registry* registry) {
  auto view = registry->view<renderable, position, collidable>();

  for (auto entity : view) {
    auto &r = view.get<renderable>(entity);
    auto &p = view.get<position>(entity);
    auto &co = view.get<collidable>(entity);
    auto s = registry->try_get<solid>(entity);
    auto &cr = co.rect;

    auto cameraView = registry->view<camera>();
    for (auto entity : cameraView) {
      camera &c = cameraView.get<camera>(entity);

      SDL_Rect dr = { (int)p.x + (int)cr.x - (int)c.pos.x, (int)p.y + (int)cr.y - (int)c.pos.y, (int)cr.w, (int)cr.h };

      if (s != nullptr) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      }
      else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      }

      SDL_RenderDrawRect(renderer, &dr);
    }
  }
}


