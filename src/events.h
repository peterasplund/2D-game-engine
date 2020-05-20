#pragma once

#include "stdafx.h"

struct collisionEvent {
  entt::registry* registry;
  entt::entity self;
  entt::entity other;
};

enum CollisionDirection {
  top,
  bottom,
  left,
  right
};

struct collisionSideEvent {
  entt::registry* registry;
  entt::entity self;
  entt::entity other;
  CollisionDirection direction;
};

struct dummyEvent {
  int foo;
};
