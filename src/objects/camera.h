#pragma once
#include "../../lib/entt/entt.hpp"
#include "../components/camera.h"
#include "../components/characterController.h"

entt::entity createCamera(entt::registry* registry, Tilemap* tilemap) {
  auto entity = registry->create();
  camera &c = registry->emplace<camera>(entity);

  c.bounds = { 0, 0, tilemap->getWidthInPixels(), tilemap->getHeightInPixels() };

  return entity;
}
