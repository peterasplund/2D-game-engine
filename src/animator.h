#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "animation.h"
#include <map>
#include <string>

class Animator
{
  public:
    Animator() {
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

    void play() {
      if (_animations[_currentAnimation] != nullptr) {
        _animations[_currentAnimation]->play();
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

    std::string getCurrent() {
      return _currentAnimation;
    }

    SDL_Rect getFrame() {
      if (_animations[_currentAnimation] != nullptr) {
        return _animations[_currentAnimation]->getFrame();
      }

      return {};
    }

  private:
    std::map<std::string, Animation*> _animations;
    std::string _currentAnimation;
};
