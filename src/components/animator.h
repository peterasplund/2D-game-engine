#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <map>
#include "../timer.h"
#include "../animation.h"

struct animator {
  std::map<std::string, Animation*> _animations;
  std::string _currentAnimation;
  Timer* timer = new Timer();
};
