#include "ldtk.h"
#include "assetManager.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>

int max(int a, int b);

using json = nlohmann::json;

LayerType strToLayerType(const char* str) {
  if (strcmp(str, "Entities") == 0) {
    return LayerType::ENTITIES;
  }
  else if (strcmp(str, "IntGrid") == 0) {
    return LayerType::INT_GRID;
  }
  else if (strcmp(str, "Tiles") == 0) {
    return LayerType::TILES;
  }

  return LayerType::TILES;
}

Layer parse_layer_instance(World* world, Level* level, json layerJson) {
  Layer layer;

  int layerDefId = layerJson["layerDefUid"];
  layer.uid = layerDefId;
  layer.def = &world->layerDefs[layerDefUidMap[layerDefId]];
  layer.level = level;

  int tileSize = layer.level->tileSize;

  if (layer.def->type == LayerType::INT_GRID) {
    layer.tiles.resize(level->tilesTall * level->tilesWide);
    for(json tile : layerJson["autoLayerTiles"]) {
      int x = (int)tile["px"][0];
      int y = (int)tile["px"][1];
      int id = tile["t"];
      int idx = (level->tilesWide * (y / tileSize)) + (x / tileSize);
      bool solid = world->tilesetDefs[layer.def->tilesetId].tileHasTag(id, "Solid");
      SDL_RendererFlip flip = tile["f"];

      Tile layerTile(id, flip, true, solid);
      layer.tiles[idx] = layerTile;
    }
  } 
  else if (layer.def->type == LayerType::TILES) {
    layer.tiles.resize(level->tilesTall * level->tilesWide);
    for (uint16_t i = 0; i < layerJson["gridTiles"].size(); i++) {
      json tile = layerJson["gridTiles"][i];
      int x = tile["px"][0];
      int y = tile["px"][1];
      uint16_t id = tile["t"];
      int idx = (level->tilesWide * (y / tileSize)) + (x / tileSize);
      bool solid = world->tilesetDefs[layer.def->tilesetId].tileHasTag(id, "Solid");
      SDL_RendererFlip flip = tile["f"];

      Tile layerTile(id, flip, true, solid);
      layer.tiles[idx] = layerTile;
    }
  }
  else if (layer.def->type == LayerType::ENTITIES) {
    for(json entityInstanceJson : layerJson["entityInstances"]) {
      Entity entity;

      entity.uid = entityUidMap[entityInstanceJson["defUid"]];
      entity.identifier = entityInstanceJson["__identifier"]; // remove this?
      entity.position = { entityInstanceJson["px"][0], entityInstanceJson["px"][1] };

      for(auto field : entityInstanceJson["fieldInstances"]) {
        EntityFieldValue fieldValue;
        int defUid = field["defUid"];;
        fieldValue.field = &world->entityDefs[entityUidMap[entity.uid]].fields[defUid];
        switch (fieldValue.field->type) {
          case Entity_Field_Tag::String:
            fieldValue.identifier = field["__identifier"];
            fieldValue.value = field["__value"];
            entity.fieldValues.push_back(fieldValue);
            break;
          case Entity_Field_Tag::Integer:
            // @TODO: implement when needed
            break;
          case Entity_Field_Tag::Float:
            // @TODO: implement when needed
            break;
        }
      }
      layer.entities.push_back(entity);

       std::sort(begin(layer.entities), end(layer.entities), [](const Entity &a, const Entity &b) {
        return a.identifier == "Player" ? 1 : -1;
      });
    }
  }

  return layer;
}

Tileset parse_tileset(std::string projectPath, json data) {
  // https://ldtk.io/json/#ldtk-TilesetDefJson

  int tileSize = data["tileGridSize"];
  // @TODO: handle spacing in tilesets?
  // int spacing = data["spacing"];
  int imageWidth = data["pxWid"];
  std::string relPath = data["relPath"];

  std::string path = projectPath + relPath;
  auto texture = AssetManager::Instance()->getTexture(path);

  if (texture == nullptr) {
    printf("ERROR: Failed to load the tileset texture at: %s\n", path.c_str());
  }

  std::map<std::string, std::vector<int>> tags;

  for(json tag : data["enumTags"]) {
    std::string enumName = tag["enumValueId"];
    std::vector<int> tileIds;

    for(int id : tag["tileIds"]) {
      tileIds.push_back(id);
    }

    // Sort the ids so we can do a binary search later on
    std::sort(tileIds.begin(), tileIds.end());

    tags[enumName] =  tileIds;
  }

  Tileset tileset;
  tileset.id = data["uid"];
  tileset.textureWidth = imageWidth;
  tileset.tileSize = tileSize;
  tileset.texture = texture;
  tileset.tags = tags;

  return tileset;
}

EntityDef parse_entity_def(json data) {
  EntityDef entity;

  entity.uid = data["uid"];
  entity.identifier = data["identifier"];

  for(json fieldJson : data["fieldDefs"]) {
    Entity_Field field;

    field.uid = fieldJson["uid"];
    field.identifier = fieldJson["identifier"];

    if (fieldJson["__type"] == "String") {
      field.type = Entity_Field_Tag::String;
    } else if (fieldJson["__type"] == "Integer") {
      field.type = Entity_Field_Tag::Integer;
    }
    // @TODO: add more field types

    entity.fields[field.uid] = field;
  }

  return entity;
}

json loadProjectFile(const char* filename) {
  std::ifstream f(filename);

  if (!f.is_open()) {
    printf("ERROR: Failed to load the tileset file at: %s\n", filename);
  }

  json data = json::parse(f);

  return data;
}

World createWorld(std::string filePath) {
  std::string projectPath = filePath.substr(0, filePath.rfind('/')) + '/';
  json data = loadProjectFile(filePath.c_str());
  World world;

  world.tileSize = data["defaultGridSize"];
  world.worldCellWidth = (int)data["worldGridWidth"] / world.tileSize;
  world.worldCellHeight = (int)data["worldGridHeight"] / world.tileSize;

  printf("> parsing tileset defs\n");

  int i = 0;
  for(json tilesetDefJson : data["defs"]["tilesets"]) {
    std::string identifier = tilesetDefJson["identifier"];

    if (identifier != "Internal_Icons") {
      Tileset tileset = parse_tileset(projectPath, tilesetDefJson);
      tilesetUidMap[tileset.id] = i;
      world.tilesetDefs.push_back(tileset);
      i ++;
    }
  }

  i = 0;
  printf("> parsing entity defs\n");
  for(json entityDefJson : data["defs"]["entities"]) {
    EntityDef entity = parse_entity_def(entityDefJson);
    // entity.uid
    entityUidMap[entity.uid] = i;
    world.entityDefs.push_back(entity);
    i ++;
  }

  i = 0;
  printf("> parsing layer defs\n");
  for(json layerDefJson : data["defs"]["layers"]) {
    std::string type = layerDefJson["type"];

    LayerDef layerDef;
    layerDef.uid = layerDefJson["uid"];
    layerDef.identifier = layerDefJson["identifier"];
    layerDef.tilesetId = layerDefJson["tilesetDefUid"].is_null() 
      ? -1 
      : tilesetUidMap[layerDefJson["tilesetDefUid"]];
    layerDef.type = strToLayerType(type.c_str());

    layerDefUidMap[layerDefJson["uid"]] = i;
    world.layerDefs.push_back(layerDef);
    i ++;
  }

  printf("> parsing levels\n");
  i = 0;
  for(json levelJson : data["levels"]) {
    Level level;

    std::string identifier = levelJson["identifier"];
    level.iid = i;
    int tileSize = data["defaultGridSize"];
    level.tilesWide = (int)levelJson["pxWid"] / tileSize;
    level.tilesTall = (int)levelJson["pxHei"] / tileSize;
    level.tileSize = tileSize;

    printf("> parsing level %s layer instances\n", identifier.c_str());
    for(json layerJson : levelJson["layerInstances"]) {
      level.layers.push_back(parse_layer_instance(&world, &level, layerJson));
    }

    level.cellPositionPx = {
      (int)levelJson["worldX"],
      (int)levelJson["worldY"],
    };

    // Save global world position
    level.cellPosition = {
      (level.cellPositionPx.x / world.worldCellWidth) / world.tileSize,
      (level.cellPositionPx.y / world.worldCellHeight) / world.tileSize,
    };

    // Save global world position
    level.cellSize = {
      level.tilesWide / world.worldCellWidth,
      level.tilesTall / world.worldCellHeight,
    };

    // neighbours
    for(json neighbourJson : levelJson["__neighbours"]) {
      std::string dirString = neighbourJson["dir"];
      std::string iid = neighbourJson["levelIid"];

      for(char d : dirString) {
        NeighBourDirection dir = neighbourDirectionFromLetter(d);
        level.neighbours[dir].push_back(iid);
      }
    }

    level.world = &world;
    world.levels.push_back(level);
    levelUidMap[levelJson["iid"]] = i;
    i ++;
  }

  int xMax = 0;
  int yMax = 0;
  for(auto level : world.levels) {
    xMax = max(xMax, level.cellSize.x + level.cellPosition.x);
    yMax = max(yMax, level.cellSize.y + level.cellPosition.y);
  }

  // Allocate for the levelsByCells lookup
  world.levelsByCells = (Level ***)malloc(xMax * sizeof(Level**));
  for (int i = 0; i < xMax; i++) {
    world.levelsByCells[i] = (Level**)malloc(yMax * sizeof(Level*));
  }

  for(Level level : world.levels) {
    for (int x = 0; x < level.cellSize.x; x ++) {
      for (int y = 0; y < level.cellSize.y; y ++) {
        printf("insert at: (%d, %d): %d\n", level.cellPosition.x + x, level.cellPosition.y + y, level.iid);
        world.levelsByCells[level.cellPosition.x + x][level.cellPosition.y + y] = &world.levels[level.iid];
      }
    }
  }

  printf("Successfully loaded the world map\n\n");

  return world;
}
