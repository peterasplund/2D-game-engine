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

    bool setAnimation(std::string name);

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

    bool isPlaying() {
      if (_animations[_currentAnimation] == nullptr) {
        return true;
      }

      return _animations[_currentAnimation]->isPlaying();
    }

    std::string getCurrent() {
      return _currentAnimation;
    }

    SDL_Texture* getTexture() {
      return _animations[_currentAnimation]->getTexture();
    }

    Rect getFrame();

  private:
    std::map<std::string, Animation*> _animations;
    std::string _currentAnimation;
    Timer* timer;
};
