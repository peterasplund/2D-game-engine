#pragma once
#include "../stdafx.h"
#include "../assetManager.h"
#include "../math.h"
#include "../components/position.h"
#include "../components/velocity.h"
#include "../components/renderable.h"
#include "../components/collidable.h"
#include "../components/solid.h"

void createMovingPlatform(SDL_Renderer* renderer, v2 initPosition, v2 v) {

  SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("tileset.png");

  // @TODO: get size from texture
  SDL_Rect size = { (int)0.0f, (int)0.0f, (int)32.0f, (int)32.0f };
  SDL_Rect textureRect = { (int)0.0f, (int)224.0f, (int)32.0f, (int)32.0f };

  auto entity = registry.create();
  registry.emplace<position>(entity, initPosition.x, initPosition.y);
  registry.emplace<velocity>(entity, v.x, v.y, 0.0f);
  registry.emplace<renderable>(entity, texture, textureRect);
  registry.emplace<collidable>(entity, size);
  registry.emplace<solid>(entity, false);
}
