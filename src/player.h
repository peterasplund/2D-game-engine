#pragma once
#include <SDL2/SDL.h>
#include "assetManager.h"
#include "object.h"
#include "animation.h"
#include "animator.h"
#include "inputHandler.h"
#include "camera.h"

class Player : public Object
{
private:
  Animator animator;
  InputHandler* _inputHandler;
public:
  Player(SDL_Renderer* renderer) : Object() {
    size = { 32.0f, 50.0f };
    speed = 2.0;

    _inputHandler = InputHandler::Instance();
    texture = AssetManager::Instance(renderer)->getTexture("sprites/character_ness_walk.png");

    textureRect.w = 32;
    textureRect.h = 50;
    textureRect.x = 0;
    textureRect.y = 0;

    int tw = 32;
    int th = 50;

    Animation* animLeft = new Animation();
    Animation* animRight = new Animation();
    Animation* animUp = new Animation();
    Animation* animDown = new Animation();

    animLeft->addFrame({ tw * 0, th * 3, tw, th }, 100);
    animLeft->addFrame({ tw * 1, th * 3, tw, th }, 100);
    animLeft->addFrame({ tw * 2, th * 3, tw, th }, 100);
    animLeft->addFrame({ tw * 3, th * 3, tw, th }, 100);

    animRight->addFrame({ tw * 0, th * 5, tw, th }, 100);
    animRight->addFrame({ tw * 1, th * 5, tw, th }, 100);
    animRight->addFrame({ tw * 2, th * 5, tw, th }, 100);
    animRight->addFrame({ tw * 3, th * 5, tw, th }, 100);

    animUp->addFrame({ tw * 0, th * 7, tw, th }, 100);
    animUp->addFrame({ tw * 1, th * 7, tw, th }, 100);
    animUp->addFrame({ tw * 2, th * 7, tw, th }, 100);
    animUp->addFrame({ tw * 3, th * 7, tw, th }, 100);

    animDown->addFrame({ tw * 0, th * 1, tw, th }, 100);
    animDown->addFrame({ tw * 1, th * 1, tw, th }, 100);
    animDown->addFrame({ tw * 2, th * 1, tw, th }, 100);
    animDown->addFrame({ tw * 3, th * 1, tw, th }, 100);

    animator.addAnimation("left", animLeft);
    animator.addAnimation("right", animRight);
    animator.addAnimation("up", animUp);
    animator.addAnimation("down", animDown);

    animator.setAnimation("left");
  }

  void update(float dt) {
    velocity.x = 0.0f;
    velocity.y = 0.0f;

    if (_inputHandler->isPressed(BUTTON::LEFT)) {
      velocity.x = -1.0f;
    }

    if (_inputHandler->isPressed(BUTTON::RIGHT)) {
      velocity.x = 1.0f;
    }

    if (_inputHandler->isPressed(BUTTON::UP)) {
      velocity.y = -1.0f;
    }

    if (_inputHandler->isPressed(BUTTON::DOWN)) {
      velocity.y = 1.0f;
    }

    if (velocity.x > 0) {
      animator.setAnimation("right");
    }
    if (velocity.x < 0) {
      animator.setAnimation("left");
    }

    if (velocity.y > 0) {
      animator.setAnimation("down");
    }
    if (velocity.y < 0) {
      animator.setAnimation("up");
    }

    if (velocity.x == 0.0f && velocity.y == 0.0f) {
      animator.stop();
      animator.reset();
    } else {
      animator.play();
    }

    textureRect = animator.getFrame();

    Object::update(dt);
  }
};
