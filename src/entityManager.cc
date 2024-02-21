#include "entityManager.h"
#include "abstractGameobject.h"

EntityManager::EntityManager() {
  _entities = std::vector<std::unique_ptr<AbstractGameObject>>();
}

EntityManager* EntityManager::Instance() {
  static EntityManager* _instance = nullptr;
  if (_instance == nullptr) {
    _instance = new EntityManager();
  }

  return _instance;
}

void EntityManager::addEntity(std::unique_ptr<AbstractGameObject> x) {
  _entities.push_back(std::move(x));
}

void EntityManager::setTileMap(Tilemap* x) {
_tilemap = x;
}

std::vector<std::unique_ptr<AbstractGameObject>>* EntityManager::getEntities() {
  return &_entities;
}

// This is a bit naive without a good data structure but we'll be fine.
AbstractGameObject* EntityManager::getEntityByTag(OBJECT_TAG tag) {
  for(const auto &obj : _entities) {
    if (obj->getTag() == tag) {
      return &(*obj); // lol
    }
  }

  printf("ERROR: Tried getting an entity by a tag that doesn't exist. Tag: %d\n", tag);
  return nullptr;
}

/*
std::vector<std::unique_ptr<AbstractGameObject>> EntityManager::getEntitiesByTag(OBJECT_TAG tag) {
  std::vector<std::unique_ptr<AbstractGameObject>> filtered;

  for(const auto &obj : _entities) {
    if (obj->getTag() == tag) {
      filtered.push_back(obj);
    }
  }

  return filtered;
}
*/

Tilemap* EntityManager::getTilemap() {
  return _tilemap;
}

void EntityManager::release() {
}
