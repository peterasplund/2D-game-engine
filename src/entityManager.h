#pragma once

#include <memory>
#include "LDtkParser.h"
#include "globals.h"

#define MAX_ENTITIES 1024

class AbstractGameObject;

/*
 * This class will own the entities and handle allocations and such
 */
class EntityManager {
  public:
    std::vector<std::shared_ptr<AbstractGameObject>> _entities;
    LDTK_Level* _tilemap;
    EntityManager();

    static EntityManager* Instance();

    void addEntity(std::shared_ptr<AbstractGameObject> x);

    void setTileMap(LDTK_Level* x);

    const std::vector<std::shared_ptr<AbstractGameObject>>& getEntities();
    // std::vector<std::shared_ptr<AbstractGameObject>> getEntitiesByTag(OBJECT_TAG tag);
    /// Gets the first object it finds by tag
    std::shared_ptr<AbstractGameObject> getEntityByTag(OBJECT_TAG tag);

    LDTK_Level* getTilemap();

    static void release();
};
