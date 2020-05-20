#pragma once

#include "stdafx.h"
#include <iostream>
#include <functional>
#include <map>
#include <vector>

typedef std::function<bool(SDL_Event *)> cbEvent;

class EventHandler {
  std::map<Uint32, std::vector<cbEvent>> _events;
public:
  bool handleEvents() {
    SDL_Event event;
    bool result;

    while (SDL_PollEvent(&event)) {
      if (_events.find(event.type) != _events.end()) {
        for (cbEvent evt : _events[event.type]) {
          result = evt(&event);
          if (!result) {
            return false;
          }
        }
      }
    }

    return true;
  }

  void registerEvent(Uint32 evtType, cbEvent evt) {
    _events[evtType].push_back(evt);
  }
};
