#pragma once
#include "../../lib/entt/entt.hpp"
#include "../assetManager.h"
#include "../math.h"
#include "../components/bat.h"
#include "../components/position.h"
#include "../components/velocity.h"
#include "../components/renderable.h"
#include "../components/animator.h"
#include "../components/characterController.h"
#include "../components/gravity.h"
#include "../components/collidable.h"
#include "../components/hurtable.h"

entt::entity createBat(entt::registry* registry, SDL_Renderer* renderer, v2 initPosition, v2 initVelocity) {

  SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/bat.png");

  int tw = 16;
  int th = 16;

  Animation* animFly = new Animation(texture);

  animFly->addFrame({ tw * 0, th * 0, tw, th }, 80);
  animFly->addFrame({ tw * 1, th * 0, tw, th }, 80);
  animFly->addFrame({ tw * 2, th * 0, tw, th }, 80);
  animFly->addFrame({ tw * 3, th * 0, tw, th }, 80);
  animFly->addFrame({ tw * 4, th * 0, tw, th }, 80);

  std::map <std::string, Animation*> animations {
    std::make_pair("fly", animFly),
  };

  SDL_Rect collisionBox = { (int)2.0f, (int)1.0f, (int)13.0f, (int)8.0f };

  auto entity = registry->create();
  registry->emplace<bat>(entity);
  registry->emplace<position>(entity, initPosition.x - collisionBox.w, initPosition.y - collisionBox.h);
  registry->emplace<velocity>(entity, initVelocity.x, initVelocity.y, 0.0f);
  registry->emplace<animator>(entity, animations, "fly");
  registry->emplace<collidable>(entity, collisionBox);
  registry->emplace<hurtable>(entity);
  registry->emplace<renderable>(entity, texture, collisionBox, SDL_FLIP_HORIZONTAL);

  return entity;
}
