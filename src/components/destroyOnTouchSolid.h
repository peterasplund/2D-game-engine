#pragma once
#include "../stdafx.h"
#include "../events.h"

struct destroyOnTouchSolid {
};


void destroyOnTouchSolidBind() {
  struct collisionListener {
    void receive(const collisionEvent &e) {
      if (!registry.valid(e.self) || !registry.valid(e.other)) {
        return;
      }

      if (registry.has<destroyOnTouchSolid>(e.self)) {
        if (registry.has<solid>(e.other)) {
          registry.destroy(e.self);
        }
      }
    }
  };

    collisionListener listener;
    dispatcher.sink<collisionEvent>().connect<&collisionListener::receive>(listener);
}
