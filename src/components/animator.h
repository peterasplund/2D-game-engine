#pragma once
#include "../stdafx.h"
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
