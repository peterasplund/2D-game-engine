#pragma once

#include "abstractGameobject.h"
#include "assetManager.h"
#include "inputHandler.h"

class Box : public AbstractGameObject {
  public:
    virtual ~Box() {
    }

    void init(SDL_Renderer* renderer) override {
      AbstractGameObject::init(renderer);
      _gravity.entityGravity = 0.0f;

      this->_collidable.boundingBox = { 
        0, 0, 
        16, 16
      };

      setListenForCollisions();

      SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/test-14x.png");

      this->_renderable.texture = texture;
      this->_renderable.textureRect = this->_collidable.boundingBox;
      this->_position = {64,64};
      // this->_velocity.friction = 5.0f;
    }

    void update(float dt) override {
      InputHandler* inputHandler = InputHandler::Instance();

      if (inputHandler->isHeld(BUTTON::LEFT)) {
        _velocity.x = -5.0f;
      }
      else if (inputHandler->isHeld(BUTTON::RIGHT)) {
        _velocity.x = 5.0f;
      }
      if (inputHandler->isHeld(BUTTON::UP)) {
        _velocity.y = -5.0f;
      }
      else if (inputHandler->isHeld(BUTTON::DOWN)) {
        _velocity.y = 5.0f;
      }

      AbstractGameObject::update(dt);

      _velocity.x = 0;
      _velocity.y = 0;
    }

    void draw(SDL_Renderer* renderer, v2 offset) override {
      AbstractGameObject::draw(renderer, offset);
    }
};
