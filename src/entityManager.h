#pragma once

#include "abstractGameobject.h"
#include "tilemap.h"
#include <vector>

class EntityManager {
  static EntityManager* _instance;
  public:
    //std::vector<AbstractGameObject> _entities;
    Tilemap* _tilemap;

    static EntityManager* Instance() {
    if (_instance == NULL) {
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

    void setTileMap(Tilemap* x) {
      _tilemap = x;
    }

    /*
    std::vector<AbstractGameObject>* getEntities() {
      return &_entities;
    }
    */

    Tilemap* getTilemap() {
      return _tilemap;
    }

    static void release() {
      delete _instance;
      _instance = NULL;
    }
};

EntityManager* EntityManager::_instance = 0;
