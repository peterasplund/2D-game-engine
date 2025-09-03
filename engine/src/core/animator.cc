#include "animator.h"
#include "logger.h"

bool Animator::setAnimation(std::string name) {
  if (_animations[name] != nullptr) {

    if (_currentAnimation != name) {
      _currentAnimation = name;
      // reset animation frame when changing animation
      timer.reset();
      _animations[_currentAnimation]->reset();
    }
    else {
      _currentAnimation = name;
    }

    return true;
  }

  LOG_WARN("Cannot play animation by name: \"%s\"", name.c_str());
  return false;
}

Rect Animator::getFrame() {
  if (_currentAnimation.empty()) {
    LOG_WARN("Error: No animation available at _currentAnimation");
    return {};
  }

  if (_animations[_currentAnimation] != nullptr) {
    return _animations[_currentAnimation]->getFrame(&timer);
  }

  LOG_WARN("Error: No animation available at _currentAnimation");
  return {};
}
