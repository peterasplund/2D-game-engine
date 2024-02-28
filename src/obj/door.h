#pragma once

#include "../abstractGameobject.h"
#include "../animator.h"
#include "../assetManager.h"
#include "../inputHandler.h"
#include "../renderer.h"

namespace obj {
  class Door : public AbstractGameObject {
    public:
      void init() override {
        AbstractGameObject::init();

        this->_collidable.boundingBox = { 
          0, 0, 
          16, 32 
        };

        SDL_Texture* texture = AssetManager::Instance()->getTexture("sprites/door.png");
        this->_renderable.texture = texture;
      }

      void update(float dt) override {
        // AbstractGameObject::update(dt);
      }

      void draw(Renderer* renderer) override {
        AbstractGameObject::draw(renderer);
      }

    protected:
      GAME_OBJECT _type = GAME_OBJECT::DOOR;

      std::string toLevel;
      v2f toLevelPlayerPosition;
  };
}
