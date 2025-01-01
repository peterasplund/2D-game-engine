#pragma once

#include "engine/map.h"
#include "globals.h"
#include "components/camera.h"
#define MAX_ENTITIES 1024

class AbstractGameObject;

/*
 * This class will own the entities and handle allocations and such
 */
class EntityManager {
public:
  std::vector<AbstractGameObject *> _entities;
  Level *_tilemap;
  EntityManager();

  static EntityManager *Instance();

  void update();
  void addEntity(AbstractGameObject *x);

  void setTileMap(Level *x);

  AbstractGameObject* instantiateGameObject(GAME_OBJECT obj);
  void instantiateLevelEntitites(World* world, Level *level);

  const std::vector<AbstractGameObject *> &getEntities();
  // std::vector<std::shared_ptr<AbstractGameObject>>
  // getEntitiesByTag(OBJECT_TAG tag);
  /// Gets the first object it finds by tag
  AbstractGameObject *getEntityByTag(OBJECT_TAG tag);

  Level *getTilemap();

  static void release();
  AbstractGameObject* _player = nullptr;
  camera _camera;
};
