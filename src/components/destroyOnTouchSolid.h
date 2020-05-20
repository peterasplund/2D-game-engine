#pragma once
#include "../stdafx.h"
#include "../events.h"

struct destroyOnTouchSolid {
};


void destroyOnTouchSolidBind(entt::dispatcher* dispatcher) {
  struct collisionListener {
    void receive(const collisionEvent &e) {
      if (!e.registry->valid(e.self) || !e.registry->valid(e.other)) {
        return;
      }

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
