#pragma once
#include "SDL.h"
#include "math.h"
#include "../../lib/entt/entt.hpp"
#include "../components/camera.h"
#include "../components/batSpawner.h"
#include "../components/renderable.h"
#include "../components/position.h"
#include "../components/bat.h"
#include "../objects/bat.h"

void batSpawnerSystem(SDL_Renderer* renderer, entt::registry* registry) {
  auto view = registry->view<batSpawner>();

  for (auto entity : view) {
    auto &e = view.get<batSpawner>(entity);

    if (e.timer.elapsed() > e.frequency) {
      auto cameraView = registry->view<camera>();
      camera* c = nullptr;
      for (auto entity : cameraView) { c = &cameraView.get<camera>(entity); }
      SDL_Rect cameraR = { c->pos.x, c->pos.y, c->viewport.x, c->viewport.y };

      if (SDL_HasIntersection(&cameraR, &c->bounds)) {
        if (c != nullptr) {

          float speed = .5f;

          v2 velocity = { 0.0f, 0.0f };
          velocity.x = e.direction == e.rightToLeft ? -speed : speed;

          auto b = createBat(registry, renderer, { 0.0f, 0.0f }, velocity );

          auto bb = registry->get<bat>(b);
          auto br = registry->get<renderable>(b);
          auto& bp = registry->get<position>(b);

          bp.y = rand() % cameraR.h + cameraR.y;
          bb.yPos = bp.y;
          bp.x = e.direction == e.rightToLeft ?
                  cameraR.x + cameraR.w - 1 :
                  cameraR.x - br.textureRect.w + 1;

          printf("spawn bat at (%f %f) \n", bp.x, bp.y);
        }
      }

      e.timer.reset();
      printf("bat timer\n");
    }
  }
}