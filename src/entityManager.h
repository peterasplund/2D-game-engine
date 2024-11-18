#pragma once

#include <memory>
#include <list>
#include "map.h"
#include "globals.h"

#define MAX_ENTITIES 1024

class AbstractGameObject;

/*
 * This class will own the entities and handle allocations and such
 */
class EntityManager {
  public:
    std::list<AbstractGameObject*> _entities;
    Level* _tilemap;
    EntityManager();

    static EntityManager* Instance();

    void update();
    void addEntity(AbstractGameObject* x);

    void setTileMap(Level* x);

    const std::list<AbstractGameObject*>& getEntities();
    // std::vector<std::shared_ptr<AbstractGameObject>> getEntitiesByTag(OBJECT_TAG tag);
    /// Gets the first object it finds by tag
    AbstractGameObject* getEntityByTag(OBJECT_TAG tag);

    Level* getTilemap();

    static void release();
};
