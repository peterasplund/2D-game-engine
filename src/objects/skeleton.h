#pragma once
#include "../../lib/entt/entt.hpp"
#include "../assetManager.h"
#include "../math.h"
#include "../tilemap.h"
#include "../components/position.h"
#include "../components/velocity.h"
#include "../components/renderable.h"
#include "../components/animator.h"
#include "../components/characterController.h"
#include "../components/gravity.h"
#include "../components/collidable.h"
#include "../components/solid.h"
#include "../components/skeleton.h"

entt::entity createSkeleton(entt::registry* registry, SDL_Renderer* renderer, TiledObject o) {
  SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/castlevania_monsters.png");

  int tw = 16;
  int th = 32;

  Animation* animWalk = new Animation(texture);

  animWalk->addFrame({ 9, 0, tw, th }, 150);
  animWalk->addFrame({ 49, 0, tw, th }, 150);

  Animation* animIdle = new Animation(texture);
  animIdle->addFrame({ 9, 0, tw, th }, 150);

  std::map <std::string, Animation*> animations {
    std::make_pair("idle", animIdle),
    std::make_pair("walk", animWalk),
  };

  SDL_Rect collisionBox = { (int)0.0f, (int)0.0f, (int)16.0f, (int)32.0f };

  auto entity = registry->create();
  registry->emplace<position>(entity, o.position.x - collisionBox.w, o.position.y - collisionBox.h);
  registry->emplace<velocity>(entity);
  registry->emplace<gravity>(entity);
  registry->emplace<animator>(entity, animations, "walk");
  registry->emplace<collidable>(entity, collisionBox);
  registry->emplace<renderable>(entity, texture, collisionBox);
  registry->emplace<solid>(entity, true);
  registry->emplace<skeleton>(entity);

  return entity;
}

