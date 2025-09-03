#pragma once

#include <core/animator.h>
#include <core/assetManager.h>
#include <core/renderer.h>
#include "../abstractGameobject.h"
#include "../dialogue.h"

namespace obj {
class Npc : public AbstractGameObject {
public:
  void init() override {
    AbstractGameObject::init();

    canInteract = true;
    _type = GAME_OBJECT::NPC;

    int tw = 75;
    int th = 75;

    this->_collidable.boundingBox = {0, 0, tw, th};

    _position.y = _position.y - th + 16;
    _position.x = _position.x - (int)(tw / 2);

    // @TODO: implement enum with different NPC designs
    SDL_Texture *texture =
        AssetManager::Instance()->getTexture("assets/sprites/fruktkungen.png");

    Animation *animIdle = new Animation(texture);
    // 13 frames
    animIdle->addFrame({tw * 0, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 1, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 2, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 3, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 4, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 5, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 6, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 7, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 8, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 9, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 10, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 11, th * 0, tw, th}, 100);
    animIdle->addFrame({tw * 12, th * 0, tw, th}, 100);

    this->_animator = Animator();
    _animator.addAnimation("idle", animIdle);
    _animator.setAnimation("idle");

    this->_renderable.texture = texture;
  }

  void setProperties(std::string name, std::string dialogue) {
    _name = name;
    _dialogue = dialogue;
  }

  void onInteract(Player* player) override {
    std::string message;
    message.append(_name);
    message.append("\n");
    message.append(_dialogue);

    Dialogue::Instance()->message(message);
  }

  void update(double dt) override { AbstractGameObject::update(dt); }

  void draw(Renderer *renderer) override {
    _renderable.textureRect = _animator.getFrame();
    _renderable.texture = _animator.getTexture();

    AbstractGameObject::draw(renderer);
  }

protected:
  Animator _animator;
  std::string _name;
  std::string _dialogue;
};
} // namespace obj
