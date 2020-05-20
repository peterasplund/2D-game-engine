#pragma once
#include "../stdafx.h"
#include "../components/position.h"
#include "../components/velocity.h"
#include "../components/renderable.h"
#include "../components/animator.h"
#include "../components/characterController.h"
#include "../components/gravity.h"
#include "../components/collidable.h"
#include "../components/gravity.h"
#include "../components/lifetime.h"
#include "../components/destroyOnTouchSolid.h"

void createKnife(entt::registry* registry, SDL_Renderer* renderer, v2 initPosition, v2 v) {

  SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/knife.png");

  int w, h;

  SDL_QueryTexture(texture, NULL, NULL, &w, &h);

  SDL_Rect size = { 0, 0, w, h };

  auto entity = registry->create();
  registry->emplace<position>(entity, initPosition.x - size.w, initPosition.y - size.h);
  registry->emplace<velocity>(entity, v.x, v.y, 0.0f);
  registry->emplace<renderable>(entity, texture, size);
  registry->emplace<collidable>(entity, size);
  registry->emplace<gravity>(entity, false, 0.0f);
  registry->emplace<lifetime>(entity, 1000.0f);
  registry->emplace<destroyOnTouchSolid>(entity);
}
