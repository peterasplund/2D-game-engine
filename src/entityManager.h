#pragma once

//#include "abstractGameobject.h"
#include "tilemap.h"
#include <vector>

class EntityManager {
  public:
    //std::vector<AbstractGameObject> _entities;
    Tilemap* _tilemap;

    static EntityManager* Instance();

    /*
    void setEntites(std::vector<AbstractGameObject> x) {
      _entities = x;
    }

    void addEntity(AbstractGameObject x) {
      _entities.push_back(x);
    }
    */

    void setTileMap(Tilemap* x);

    /*
    std::vector<AbstractGameObject>* getEntities() {
      return &_entities;
    }
    */

    Tilemap* getTilemap();

    static void release();
};
