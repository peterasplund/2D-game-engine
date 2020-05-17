#pragma once

#include "../../lib/entt/entt.hpp"
#include "../events.h"

struct destroyOnTouchSolid {
};


void destroyOnTouchSolidBind(entt::dispatcher* dispatcher) {
  struct collisionListener {
    void receive(const collisionEvent &e) {
      if (e.registry->has<destroyOnTouchSolid>(e.self)) {
      if (e.registry->has<solid>(e.other)) {
        e.registry->destroy(e.self);
      }
      }
    }
  };

    collisionListener listener;
    dispatcher->sink<collisionEvent>().connect<&collisionListener::receive>(listener);
}
