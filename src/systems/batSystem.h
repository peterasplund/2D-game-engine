#pragma once
#include "../stdafx.h"
#include "math.h"
#include "../components/camera.h"
#include "../components/bat.h"
#include "../components/position.h"
#include "../components/velocity.h"
#include "../components/renderable.h"

void batSystem() {
  auto view = registry.view<bat, position, velocity, renderable>();

  auto cameraView = registry.view<camera>();
  camera* c = nullptr;
  for (auto entity : cameraView) { c = &cameraView.get<camera>(entity); }

  for (auto entity : view) {
    auto &b = view.get<bat>(entity);
    auto &p = view.get<position>(entity);
    auto &v = view.get<velocity>(entity);
    auto &r = view.get<renderable>(entity);

    // Move in sinus pattern
    // p.y = b.yPos - sin((p.x / 1));

    // Destroy when one and a half cameera away
    SDL_Rect batR = { (int)p.x, (int)p.y, r.textureRect.w, r.textureRect.h };
    SDL_Rect cameraR = Camera::getRect(c);
    cameraR.x -= cameraR.w / 2;
    cameraR.w += cameraR.w;
    cameraR.y -= cameraR.h / 2;
    cameraR.h += cameraR.h;
    if (!SDL_HasIntersection(&batR, &cameraR)) {
      registry.destroy(entity);
    }
  }
}