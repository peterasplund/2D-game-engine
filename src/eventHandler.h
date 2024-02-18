#pragma once

#include "stdafx.h"
#include <functional>
#include "abstractGameobject.h"

typedef std::function<bool(SDL_Event *)> cbEvent;

class EventHandler {
  std::map<Uint32, std::vector<cbEvent>> _events;
  std::vector<AbstractGameObject*> _gameObjects;
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

        // Call handleEvent on every game object. 
        // (Redo this so the object can listen for specific events)
        for(AbstractGameObject* o : _gameObjects) {
          _gameObjects[0]->handleEvent(&event);
        }
      }
    }

    return true;
  }

  void registerEvent(Uint32 evtType, cbEvent evt) {
    _events[evtType].push_back(evt);
  }
};
