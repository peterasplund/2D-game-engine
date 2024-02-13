#include "entityManager.h"

EntityManager* EntityManager::Instance() {
  static EntityManager* _instance = nullptr;
  if (_instance == nullptr) {
    _instance = new EntityManager();
  }

  return _instance;
}

/*
void setEntites(std::vector<AbstractGameObject> x) {
  _entities = x;
}

void addEntity(AbstractGameObject x) {
  _entities.push_back(x);
}
*/

void EntityManager::setTileMap(Tilemap* x) {
_tilemap = x;
}

/*
std::vector<AbstractGameObject>* getEntities() {
  return &_entities;
}
*/

Tilemap* EntityManager::getTilemap() {
return _tilemap;
}

void EntityManager::release() {
}
