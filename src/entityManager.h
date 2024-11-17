#pragma once

#include <memory>
#include "map.h"
#include "globals.h"

#define MAX_ENTITIES 1024

class AbstractGameObject;

/*
 * This class will own the entities and handle allocations and such
 */
class EntityManager {
  public:
    std::vector<std::shared_ptr<AbstractGameObject>> _entities;
    Level* _tilemap;
    EntityManager();

    static EntityManager* Instance();

    void addEntity(std::shared_ptr<AbstractGameObject> x);
    void clearAllButConstantEntities();

    void setTileMap(Level* x);

    const std::vector<std::shared_ptr<AbstractGameObject>>& getEntities();
    // std::vector<std::shared_ptr<AbstractGameObject>> getEntitiesByTag(OBJECT_TAG tag);
    /// Gets the first object it finds by tag
    int getEntityIdxByTag(OBJECT_TAG tag);
    std::shared_ptr<AbstractGameObject> getEntityByTag(OBJECT_TAG tag);

    Level* getTilemap();

    static void release();
};
