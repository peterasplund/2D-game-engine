#include "entityManager.h"
#include "abstractGameobject.h"
#include "logger.h"

EntityManager::EntityManager() {
  _entities = std::list<AbstractGameObject*>();
}

EntityManager* EntityManager::Instance() {
  static EntityManager* _instance = nullptr;
  if (_instance == nullptr) {
    _instance = new EntityManager();
  }

  return _instance;
}

void EntityManager::addEntity(AbstractGameObject* x) {
  _entities.push_back(std::move(x));
}

void EntityManager::setTileMap(Level* x) {
  _tilemap = x;
}

const std::list<AbstractGameObject*>& EntityManager::getEntities() {
  return _entities;
}

// This is a bit naive without a good data structure but we'll be fine.
AbstractGameObject* EntityManager::getEntityByTag(OBJECT_TAG tag) {
  for(const auto &obj : _entities) {
    if (obj->getTag() == tag) {
      return obj;
    }
  }

  LOG_WARN("ERROR: Tried getting an entity by a tag that doesn't exist. Tag: %d\n", (int)tag);
  return nullptr;
}

Level* EntityManager::getTilemap() {
  return _tilemap;
}

void EntityManager::release() {
  auto i = Instance()->_entities;
  std::list<AbstractGameObject*>::iterator it;
  for (it = Instance()->_entities.begin(); it != Instance()->_entities.end();){
    delete (*it);
    it = Instance()->_entities.erase(it);
  }
}

void EntityManager::update() {
  std::list<AbstractGameObject*>::iterator it;
  for (it = _entities.begin(); it != _entities.end();){
    if ((*it)->dead) {
      delete (*it);
      it = _entities.erase(it);
    }
    else {
      ++it;
    }
  }
}
