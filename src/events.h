#pragma once

#include "../lib/entt/entt.hpp"

struct collisionEvent {
  entt::registry* registry;
  entt::entity self;
  // entt::entity other;
};

struct dummyEvent {
  int foo;
};