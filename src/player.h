#pragma once
#include <SDL2/SDL.h>
#include "math.h"
#include "assetManager.h"
#include "object.h"
#include "animation.h"
#include "animator.h"
#include "inputHandler.h"

class Player : Object
{
private:
  SDL_Texture* texture;
  SDL_Rect rect;
  SDL_Rect texRect;

  v2 position = { 0.0f, 0.0f };
  v2 velocity = { 0.0f, 0.0f };
  v2 size = { 32.0f, 50.0f };

  float speed = 2.0;

  Animator animator;
  SDL_Event event;
  InputHandler* _inputHandler;
public:
  Player(SDL_Renderer* renderer) : Object(renderer) {
    _inputHandler = InputHandler::Instance();
    texture = AssetManager::Instance(renderer)->getTexture("sprites/character_ness_walk.png");

    texRect.w = 32;
    texRect.h = 50;
    texRect.x = 0;
    texRect.y = 0;

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

    position.x += (velocity.x / 10) * speed * dt;
    position.y += (velocity.y / 10) * speed * dt;

    // clamp
    if (position.x < 0) {
      position.x = 0;
    } else if (position.x > 640 - size.x) {
      position.x = 640 - size.x;
    }
    if (position.y < 0) {
      position.y = 0;
    } else if (position.y > 480 - size.y) {
      position.y = 480 - size.y;
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

  }

  void draw(SDL_Renderer* renderer) {
    rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };

    texRect = animator.getFrame();

    SDL_RenderCopy(renderer, texture, &texRect, &rect);

    SDL_RenderPresent(renderer);
    printf("w: %d h: %d\n", texRect.w, texRect.h);
  }
};
