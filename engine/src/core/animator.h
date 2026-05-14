#pragma once

#include "animation.h"
#include "math.h"
#include "timer.h"
#include <map>
#include <memory>
#include <string>

class Animator {
public:
  Animator() { timer = Timer(); }

  ~Animator() = default;
  Animator(const Animator &) = delete;
  Animator &operator=(const Animator &) = delete;
  Animator(Animator &&) = default;
  Animator &operator=(Animator &&) = default;

  void addAnimation(std::string name, std::unique_ptr<Animation> animation) {
    _animations[name] = std::move(animation);
  }

  bool setAnimation(std::string name);

  void start() {
    if (auto *animation = currentAnimation()) {
      animation->start();
    }
  }

  void stop() {
    if (auto *animation = currentAnimation()) {
      animation->stop();
    }
  }

  void reset() {
    if (auto *animation = currentAnimation()) {
      animation->reset();
    }
  }

  bool hasPlayedThrough() {
    if (auto *animation = currentAnimation()) {
      return animation->hasPlayedThrough();
    }

    return true;
  }

  bool isPlaying() {
    if (auto *animation = currentAnimation()) {
      return animation->isPlaying();
    }

    return true;
  }

  std::string getCurrent() { return _currentAnimation; }

  SDL_Texture *getTexture() {
    if (auto *animation = currentAnimation()) {
      return animation->getTexture();
    }

    return nullptr;
  }

  Rect getFrame();

private:
  Animation *currentAnimation() {
    auto it = _animations.find(_currentAnimation);
    if (it == _animations.end()) {
      return nullptr;
    }

    return it->second.get();
  }

  const Animation *currentAnimation() const {
    auto it = _animations.find(_currentAnimation);
    if (it == _animations.end()) {
      return nullptr;
    }

    return it->second.get();
  }

  std::map<std::string, std::unique_ptr<Animation>> _animations;
  std::string _currentAnimation;
  Timer timer;
};
