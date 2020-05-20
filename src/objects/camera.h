#pragma once
#include "../stdafx.h"
#include "../components/camera.h"
#include "../components/characterController.h"

entt::entity createCamera(Tilemap* tilemap) {
  auto entity = registry.create();
  camera &c = registry.emplace<camera>(entity);

  c.bounds = { 0, 0, tilemap->getWidthInPixels(), tilemap->getHeightInPixels() };

  return entity;
}
