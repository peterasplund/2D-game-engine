#pragma once

#include <string>
#include <map>
#include "animation.h"

class Animator
{
  public:
    Animator() {
      timer = new Timer();
    }

    ~Animator() {
    }

    void addAnimation(std::string name, Animation* animation) {
      _animations[name] = animation;
    }

    bool setAnimation(std::string name) {
      if (_animations[name] != nullptr) {
        _currentAnimation = name;

        if (_currentAnimation != name) {
          // reset animation frame when changing animation
          _animations[_currentAnimation]->reset();
        }
        return true;
      }

      return false;
    }

    void start() {
      if (_animations[_currentAnimation] != nullptr) {
        _animations[_currentAnimation]->start();
      }
    }

    void stop() {
      if (_animations[_currentAnimation] != nullptr) {
        _animations[_currentAnimation]->stop();
      }
    }

    void reset() {
      if (_animations[_currentAnimation] != nullptr) {
        _animations[_currentAnimation]->reset();
      }
    }

    bool hasPlayedThrough() {
      if (_animations[_currentAnimation] == nullptr) {
        return true;
      }

      return _animations[_currentAnimation]->hasPlayedThrough();
    }

    std::string getCurrent() {
      return _currentAnimation;
    }

    SDL_Texture* getTexture() {
      return _animations[_currentAnimation]->getTexture();
    }

    SDL_Rect getFrame() {
      if (_currentAnimation.empty()) {
        printf("Error: No animation available at _currentAnimation\n");
        return {};
      }

      if (_animations[_currentAnimation] != nullptr) {
        return _animations[_currentAnimation]->getFrame(timer);
      }

      printf("Error: No animation available at _currentAnimation\n");
      return {};
    }

  private:
    std::map<std::string, Animation*> _animations;
    std::string _currentAnimation;
    Timer* timer;
};
