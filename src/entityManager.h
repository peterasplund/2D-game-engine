#pragma once

#include "stdafx.h"
#include "tilemap.h"
#include "globals.h"

#define MAX_ENTITIES 1024

class AbstractGameObject;

/*
 * This class will own the entities and handle allocations and such
 */
class EntityManager {
  public:
    std::vector<std::unique_ptr<AbstractGameObject>> _entities;
    Tilemap* _tilemap;
    EntityManager();

    static EntityManager* Instance();

    void addEntity(std::unique_ptr<AbstractGameObject> x);

    void setTileMap(Tilemap* x);

    std::vector<std::unique_ptr<AbstractGameObject>>* getEntities();
    //std::vector<std::unique_ptr<AbstractGameObject>> getEntitiesByTag(OBJECT_TAG tag);
    /// Gets the first object it finds by tag
    AbstractGameObject* getEntityByTag(OBJECT_TAG tag);

    Tilemap* getTilemap();

    void imgui();

    static void release();
};
