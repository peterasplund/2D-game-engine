#include "animator.h"
#include "logger.h"

bool Animator::setAnimation(std::string name) {
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

Rect Animator::getFrame() {
  if (_currentAnimation.empty()) {
    LOG_WARN("Error: No animation available at _currentAnimation...\n");
    return {};
  }

  if (_animations[_currentAnimation] != nullptr) {
    return _animations[_currentAnimation]->getFrame(timer);
  }

  LOG_WARN("Error: No animation available at _currentAnimation\n");
  return {};
}
