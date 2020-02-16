#pragma once
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/velocity.h"
#include "../components/renderable.h"
#include "../components/animator.h"
#include "../components/characterController.h"
#include "../components/gravity.h"
#include "../components/collidable.h"
#include "../components/gravity.h"

void createKnife(entt::registry* registry, SDL_Renderer* renderer, v2 initPosition, v2 v) {

  SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/knife.png");

  // @TODO: get size from texture
  SDL_Rect size = { (int)0.0f, (int)0.0f, (int)6.0f, (int)2.0f };

  auto entity = registry->create();
  registry->assign<position>(entity, initPosition.x - size.w, initPosition.y - size.h);
  registry->assign<velocity>(entity, v.x, v.y, 0.0f);
  registry->assign<renderable>(entity, texture, size);
  registry->assign<collidable>(entity, size);
  registry->assign<gravity>(entity, false, 0.0f);
}
