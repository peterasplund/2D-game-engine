#pragma once

#include "abstractGameobject.h"
#include <vector>

class EntityManager {
  static EntityManager* _instance;
  public:
    //std::vector<AbstractGameObject> _entities;
    std::vector<std::vector<bool>> _solidTiles;

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

    void setSolidTiles(std::vector<std::vector<bool>> x) {
      _solidTiles = x;
    }

    /*
    std::vector<AbstractGameObject>* getEntities() {
      return &_entities;
    }
    */

    std::vector<std::vector<bool>>* getSolidTiles() {
      return &_solidTiles;
    }

    static void release() {
      delete _instance;
      _instance = NULL;
    }
};

EntityManager* EntityManager::_instance = 0;
