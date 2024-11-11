#include "entityManager.h"
#include "abstractGameobject.h"

EntityManager::EntityManager() {
  _entities = std::vector<std::shared_ptr<AbstractGameObject>>();
}

EntityManager* EntityManager::Instance() {
  static EntityManager* _instance = nullptr;
  if (_instance == nullptr) {
    _instance = new EntityManager();
  }

  return _instance;
}

void EntityManager::addEntity(std::shared_ptr<AbstractGameObject> x) {
  _entities.push_back(std::move(x));
}

void EntityManager::setTileMap(LDTK_Level* x) {
_tilemap = x;
}

const std::vector<std::shared_ptr<AbstractGameObject>>& EntityManager::getEntities() {
  return _entities;
}

// This is a bit naive without a good data structure but we'll be fine.
std::shared_ptr<AbstractGameObject> EntityManager::getEntityByTag(OBJECT_TAG tag) {
  for(const auto &obj : _entities) {
    if (obj->getTag() == tag) {
      return obj;
    }
  }

  printf("ERROR: Tried getting an entity by a tag that doesn't exist. Tag: %d\n", (int)tag);
  return nullptr;
}

/*
std::vector<std::shared_ptr<AbstractGameObject>> EntityManager::getEntitiesByTag(OBJECT_TAG tag) {
  std::vector<std::shared_ptr<AbstractGameObject>> filtered;

  for(const auto &obj : _entities) {
    if (obj->getTag() == tag) {
      filtered.push_back(obj);
    }
  }

  return filtered;
}
*/

LDTK_Level* EntityManager::getTilemap() {
  return _tilemap;
}

void EntityManager::release() {
}
