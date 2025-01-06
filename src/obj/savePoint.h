#pragma once

#include "../abstractGameobject.h"
#include "../dialogue.h"
#include "../engine/animator.h"
#include "../engine/assetManager.h"
#include "../engine/renderer.h"
#include "player.h"

namespace obj {
class SavePoint : public AbstractGameObject {
public:
  void init() override {
    AbstractGameObject::init();
    canInteract = true;
    _type = GAME_OBJECT::SAVE_POINT;

    this->_collidable.boundingBox = {0, 0, 20, 16};

    SDL_Texture *texture =
        AssetManager::Instance()->getTexture("assets/sprites/save-point.png");

    this->_renderable.texture = texture;
    this->_renderable.textureRect = {0,0,20,16};
  }

  void update(double dt) override { AbstractGameObject::update(dt); }

  void onInteract(Player* player) override {
    player->restoreHp();
    std::string message = "Game saved!";
    Dialogue::Instance()->message(message);
  }

  void draw(Renderer *renderer) override {
    AbstractGameObject::draw(renderer);
  }
};
} // namespace obj
