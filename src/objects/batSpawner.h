#pragma once
#include "../stdafx.h"
#include "../tilemap.h"
#include "../math.h"
#include "../components/position.h"
#include "../components/batSpawner.h"

entt::entity createBatSpawner(entt::registry* registry, SDL_Renderer* renderer, TiledObject o) {
  auto entity = registry->create();
  auto &bs = registry->emplace<batSpawner>(entity);

  if (o.propertiesInt.find("frequency") != o.propertiesInt.end()) {
    // printf("freq %d\n", o.propertiesInt.at("frequency"));
    bs.frequency = o.propertiesInt.at("frequency");
  }
  
  // printf("width %f\n", o.width.value());
  // printf("height %f\n", o.height.value());

  if (o.width && o.height) {
    // printf("bounds (%f\t%f\t%f\t%f)\n", o.position.x, o.position.y, o.width.value(), o.height.value());
    bs.bounds = { (int)o.position.x, (int)o.position.y, (int)o.height.value(), (int)o.width.value() };
  }
  
  return entity;
}
