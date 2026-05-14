#include "animator.h"
#include "logger.h"

bool Animator::setAnimation(std::string name) {
  auto it = _animations.find(name);
  if (it != _animations.end() && it->second != nullptr) {

    if (_currentAnimation != name) {
      _currentAnimation = name;
      // reset animation frame when changing animation
      timer.reset();
      it->second->reset();
    } else {
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

  if (auto *animation = currentAnimation()) {
    return animation->getFrame(&timer);
  }

  LOG_WARN("Error: No animation available at _currentAnimation");
  return {};
}
