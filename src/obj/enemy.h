#pragma once

#include "../abstractGameobject.h"
#include "../animator.h"
#include "../assetManager.h"
#include "../inputHandler.h"
#include "../renderer.h"

namespace obj {
  class Enemy : public AbstractGameObject {
    public:
      void init() override {
        AbstractGameObject::init();

        this->_collidable.boundingBox = { 
          35, 20, 
          60,
          112 - 50
        };

        /*
        int tw = 129;
        int th = 112;

        SDL_Texture* texture = AssetManager::Instance()->getTexture("assets/sprites/face_of_doom.png");
        */
        int tw = 75;
        int th = 75;

        SDL_Texture* texture = AssetManager::Instance()->getTexture("assets/sprites/fruktkungen.png");

        Animation* animIdle = new Animation(texture);
        // 13 frames
        animIdle->addFrame({ tw * 0, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 1, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 2, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 3, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 4, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 5, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 6, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 7, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 8, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 9, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 10, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 11, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 12, th * 0, tw, th }, 100);

        this->_animator = Animator();
        _animator.addAnimation("idle", animIdle);
        _animator.setAnimation("idle");

        this->_renderable.texture = texture;
        _position.x = 255;
        _position.y = 198;
      }

      void update(float dt) override {
        //_position.x = AMPLITUDE * sin(SPEED * elapsedTime) + 900.0f;
        //_position.y = AMPLITUDE * cos(SPEED / 2 * elapsedTime) + 50.0f;
        AbstractGameObject::update(dt);
        elapsedTime += dt;
        /*
        if (elapsedTime > 20000) {
          _position.x -= 0.08;
        }
        */
      }

      void draw(Renderer* renderer) override {
        _renderable.textureRect = _animator.getFrame();
        _renderable.texture = _animator.getTexture();

        AbstractGameObject::draw(renderer);
      }

    protected:
      GAME_OBJECT _type = GAME_OBJECT::DOOR;
      float SPEED = 0.005f;
      float AMPLITUDE = 15.0f;
      float elapsedTime = 0.0f;
      Animator _animator;
      Timer _timer;
  };
}
