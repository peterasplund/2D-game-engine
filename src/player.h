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
  bool onFloor = false;

  SDL_Rect leftRect;
  SDL_Rect rightRect;
  SDL_Rect topRect;
  SDL_Rect bottomRect;

  float _gravity = 0.035f;
  float _jumpPower = 2.2f;
  float _maxFallSpeed = 15.0f;
public:

  Player(SDL_Renderer* renderer) : Object() {
    size = { 66.0f, 48.0f };
    speed = 1.0;
    velocity = { 0, 0 };
    id = "player";

    _inputHandler = InputHandler::Instance();
    SDL_Texture* idleTexture = AssetManager::Instance(renderer)->getTexture("sprites/gothic-hero-idle.png");
    SDL_Texture* runTexture = AssetManager::Instance(renderer)->getTexture("sprites/gothic-hero-run.png");
    SDL_Texture* jumpTexture = AssetManager::Instance(renderer)->getTexture("sprites/gothic-hero-jump.png");

    int tw = 38;
    int th = 48;

    Animation* animIdle = new Animation(idleTexture);
    Animation* animRun = new Animation(runTexture);
    Animation* animJump = new Animation(jumpTexture, false);
    Animation* animFall = new Animation(jumpTexture, false);

    tw = 38;
    animIdle->addFrame({ tw * 0, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 1, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 2, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 3, th * 0, tw, th }, 500);

    tw = 66;
    animRun->addFrame({ tw * 0, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 1, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 2, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 3, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 4, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 5, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 6, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 7, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 8, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 9, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 10, th * 0, tw, th }, 100);
    animRun->addFrame({ tw * 11, th * 0, tw, th }, 100);

    tw = 61;
    th = 77;
    animJump->addFrame({ tw * 0, th * 0, tw, th }, 100);
    animJump->addFrame({ tw * 1, th * 0, tw, th }, 100);
    animJump->addFrame({ tw * 2, th * 0, tw, th }, 100);
    animJump->addFrame({ tw * 3, th * 0, tw, th }, 100);
    animJump->addFrame({ tw * 4, th * 0, tw, th }, 100);

    tw = 61;
    animFall->addFrame({ tw * 4, th * 0, tw, th }, 100);

    animator.addAnimation("idle", animIdle);
    animator.addAnimation("run", animRun);
    animator.addAnimation("jump", animJump);
    animator.addAnimation("fall", animFall);

    animator.setAnimation("fall");
  }

  void update(float dt, std::vector<Object> tiles) {
    velocity.x = 0.0f;

    leftRect = getRect();
    leftRect.w = 5;
    leftRect.h = leftRect.h / 2;
    leftRect.x = leftRect.x;
    leftRect.y = leftRect.y + (leftRect.h / 2);

    rightRect = getRect();
    rightRect.w = 5;
    rightRect.h = rightRect.h / 2;
    rightRect.x = rightRect.x + getRect().w - 5;
    rightRect.y = rightRect.y + (rightRect.h / 2);

    topRect = getRect();
    topRect.w = topRect.w / 2;
    topRect.h = 5;
    topRect.x = topRect.x + (topRect.w / 2);
    topRect.y = topRect.y;

    bottomRect = getRect();
    bottomRect.w = bottomRect.w / 2;
    bottomRect.h = 5;
    bottomRect.x = bottomRect.x + (bottomRect.w / 2);
    bottomRect.y = bottomRect.y + getRect().h - 5;

    //resolve collisions
    for (int j = 0; j < tiles.size(); j ++) {
      if (!tiles[j].getSolid()) {
        continue;
      }

      // collision
      if (Object::intersectsWith(&tiles[j])) {
        if (tiles[j].intersectsWithRect(&bottomRect) && velocity.y > 0) {
          // handle landing collision
          position.y = tiles[j].getPosition().y - size.y;
          onFloor = true;
          velocity.y = 0.0f;
        } else if (tiles[j].intersectsWithRect(&topRect) && velocity.y < 0) {
          // handle top collision
          position.y = tiles[j].getPosition().y + tiles[j].getSize().y;
          velocity.y = 0.0f;
        } else if (tiles[j].intersectsWithRect(&leftRect)) {
          // handle left collision
          position.x = tiles[j].getPosition().x + tiles[j].getSize().x;
          velocity.x = 0.0f;
        } else if (tiles[j].intersectsWithRect(&rightRect)) {
          // handle right collision
          position.x = tiles[j].getPosition().x - size.x;
          velocity.x = 0.0f;
        }
      }
    }

    if (_inputHandler->isPressed(BUTTON::LEFT)) {
      velocity.x = -1.0f;
      textureFlip = SDL_FLIP_HORIZONTAL;
    }

    if (_inputHandler->isPressed(BUTTON::RIGHT)) {
      velocity.x = 1.0f;
      textureFlip = SDL_FLIP_NONE;
    }

    if (!onFloor) {
      velocity.y += _gravity;
    }

    if (velocity.y > _maxFallSpeed) {
      velocity.y = _maxFallSpeed;
    }

    if (_inputHandler->isPressed(BUTTON::UP) && onFloor) {
      velocity.y = -_jumpPower;
      onFloor = false;
    }

    if (velocity.x == 0.0f && velocity.y == 0.0f) {
      animator.stop();
      animator.reset();
    } else {
      animator.play();
    }

    if (velocity.y > .4f && !onFloor) {
      if (animator.getCurrent() != "fall") {
        animator.reset();
        animator.setAnimation("fall");
      }
    } else if (velocity.y < 0 && !onFloor) {
      if (animator.getCurrent() != "jump") {
        animator.reset();
        animator.setAnimation("jump");
      }
    } else if (velocity.x != 0 && onFloor) {
      if (animator.getCurrent() != "run") {
        animator.setAnimation("run");
      }
    } else if (onFloor) {
      if (animator.getCurrent() != "idle") {
        animator.setAnimation("idle");
      }
    }

    onFloor = false;

    Object::update(dt);
  }


  virtual void draw(SDL_Renderer* renderer, SDL_Rect origin) {
    printf("animation: %s\n", animator.getCurrent().c_str());
    textureRect = animator.getFrame();
    setTexture(animator.getTexture());

    Object::draw(renderer, origin);

    // debug collision rectangles
    /*
    SDL_SetRenderDrawColor(renderer, 20, 255, 20, 255);
    SDL_RenderFillRect(renderer, &leftRect);
    SDL_SetRenderDrawColor(renderer, 20, 20, 255, 255);
    SDL_RenderFillRect(renderer, &rightRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 20, 255);
    SDL_RenderFillRect(renderer, &topRect);
    SDL_SetRenderDrawColor(renderer, 255, 20, 20, 255);
    SDL_RenderFillRect(renderer, &bottomRect);
    */
  }
};
