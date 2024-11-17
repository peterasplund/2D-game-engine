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

void EntityManager::setTileMap(Level* x) {
  _tilemap = x;
}

const std::vector<std::shared_ptr<AbstractGameObject>>& EntityManager::getEntities() {
  return _entities;
}

void EntityManager::clearAllButConstantEntities() {
  int playerIdx = getEntityIdxByTag(OBJECT_TAG::PLAYER);
  printf("entities count: %d\n", (int)_entities.size());

  std::vector<std::shared_ptr<AbstractGameObject>>::iterator iter = _entities.begin();

  bool a= false;
  std::vector<std::shared_ptr<AbstractGameObject>> entitiesToRemove;
  for(auto var : _entities) {
    entitiesToRemove.push_back(var);
  }
   while (iter != _entities.end()) {
    //if (!(*iter)->_persist) {
    if (!a) {
      //iter = _entities.erase(iter);
      a = true;
    }
    //}
  }
  printf("entities count: %d\n", (int)_entities.size());
}

int EntityManager::getEntityIdxByTag(OBJECT_TAG tag) {
  for(uint32_t i = 0; i < _entities.size(); i++) {
    if (_entities[i]->getTag() == tag) {
      return (int)i;
    }
  }

  printf("ERROR: Tried getting an entity by a tag that doesn't exist. Tag: %d\n", (int)tag);
  return -1;
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

Level* EntityManager::getTilemap() {
  return _tilemap;
}

void EntityManager::release() {
}
