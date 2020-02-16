#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/animator.h"
#include "../components/renderable.h"

// Where should we put these helpers?
namespace AnimationHelpers {
  void addAnimation(animator *a, std::string name, Animation* animation) {
    a->_animations[name] = animation;
  }

  bool setAnimation(animator *a, std::string name) {
    if (a->_animations[name] != nullptr) {
      a->_currentAnimation = name;

      if (a->_currentAnimation != name) {
        // reset animation frame when changing animation
        a->_animations[a->_currentAnimation]->reset();
      }
      return true;
    }

    return false;
  }

  void start(animator *a) {
    if (a->_animations[a->_currentAnimation] != nullptr) {
      a->_animations[a->_currentAnimation]->start();
    }
  }

  void stop(animator *a) {
    if (a->_animations[a->_currentAnimation] != nullptr) {
      a->_animations[a->_currentAnimation]->stop();
    }
  }

  void reset(animator *a) {
    if (a->_animations[a->_currentAnimation] != nullptr) {
      a->_animations[a->_currentAnimation]->reset();
    }
  }

  std::string getCurrent(animator *a) {
    return a->_currentAnimation;
  }

  SDL_Texture* getTexture(animator *a) {
    return a->_animations[a->_currentAnimation]->getTexture();
  }

  SDL_Rect getFrame(animator *a) {
    if (a->_animations[a->_currentAnimation] != nullptr) {
      return a->_animations[a->_currentAnimation]->getFrame(a->timer);
    }

    return {};
  }
}

void animationSystem(float dt, entt::registry* registry) {
  auto view = registry->view<animator, renderable>();

  for (auto entity : view) {
    auto &a = view.get<animator>(entity);
    auto &r = view.get<renderable>(entity);

    r.textureRect = AnimationHelpers::getFrame(&a);
    r.texture = AnimationHelpers::getTexture(&a);
  }
}
