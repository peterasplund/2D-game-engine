#pragma once
#include "../../lib/entt/entt.hpp"
#include "../components/position.h"
#include "../components/velocity.h"
#include "../components/renderable.h"
#include "../components/animator.h"
#include "../components/characterController.h"

void createPlayer(entt::registry* registry, SDL_Renderer* renderer) {

  SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/LightBandit_Spritesheet.png");

  int tw = 48;
  int th = 48;

  Animation* animIdle = new Animation(texture);
  Animation* animRun = new Animation(texture);
  Animation* animAttack = new Animation(texture, false);
  Animation* animJump = new Animation(texture, false);
  Animation* animFall = new Animation(texture, false);
  Animation* animBackDash = new Animation(texture, false);

  animIdle->addFrame({ tw * 0, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 1, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 2, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 3, th * 0, tw, th }, 500);

  animRun->addFrame({ tw * 0, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 1, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 2, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 3, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 4, th * 1, tw, th }, 100);

  animAttack->addFrame({ tw * 3, th * 2, tw, th }, 100);
  animAttack->addFrame({ tw * 4, th * 2, tw, th }, 100);
  animAttack->addFrame({ tw * 5, th * 2, tw, th }, 100);
  animAttack->addFrame({ tw * 6, th * 2, tw, th }, 100);
  animAttack->addFrame({ tw * 7, th * 2, tw, th }, 100);

  animJump->addFrame({ tw * 5, th * 4, tw, th }, 100);

  animFall->addFrame({ tw * 5, th * 4, tw, th }, 100);

  animBackDash->addFrame({ tw * 1, th * 4, tw, th }, 100);

  std::map <std::string, Animation*> animations {
    std::make_pair("idle", animIdle),
    std::make_pair("run", animRun),
    std::make_pair("attack", animAttack),
    std::make_pair("jump", animJump),
    std::make_pair("fall", animFall),
    std::make_pair("backDash", animBackDash),
  };

  auto entity = registry->create();
  registry->assign<position>(entity, 10.0f, 50.0f);
  registry->assign<velocity>(entity);
  registry->assign<renderable>(entity, texture);
  registry->assign<animator>(entity, animations, "run");
  registry->assign<characterController>(entity);
}
