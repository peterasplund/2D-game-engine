#include "ldtk.h"
#include "assetManager.h"
#include "logger.h"
#include <algorithm>
#include <fstream>
#include <vector>

using json = nlohmann::json;

LayerType strToLayerType(const char *str) {
  if (strcmp(str, "Entities") == 0) {
    return LayerType::ENTITIES;
  } else if (strcmp(str, "IntGrid") == 0) {
    return LayerType::INT_GRID;
  } else if (strcmp(str, "Tiles") == 0) {
    return LayerType::TILES;
  }

  return LayerType::TILES;
}

Layer parse_layer_instance(World *world, Level *level, json layerJson) {
  Layer layer;

  int layerDefId = layerJson["layerDefUid"];
  layer.uid = layerDefId;
  layer.def = &world->layerDefs[layerDefUidMap[layerDefId]];
  layer.level = level;

  int tileSize = layer.level->tileSize;

  if (layer.def->type == LayerType::INT_GRID) {
    layer.tiles.resize(level->tilesTall * level->tilesWide);
    for (json tile : layerJson["autoLayerTiles"]) {
      int x = (int)tile["px"][0];
      int y = (int)tile["px"][1];
      int id = tile["t"];
      int idx = (level->tilesWide * (y / tileSize)) + (x / tileSize);
      bool solid =
          world->tilesetDefs[layer.def->tilesetId].tileHasTag(id, "Solid");
      SDL_RendererFlip flip = tile["f"];

      Tile layerTile(id, flip, true, solid);
      layer.tiles[idx] = layerTile;
    }
  } else if (layer.def->type == LayerType::TILES) {
    layer.tiles.resize(level->tilesTall * level->tilesWide);
    for (uint16_t i = 0; i < layerJson["gridTiles"].size(); i++) {
      json tile = layerJson["gridTiles"][i];
      int x = tile["px"][0];
      int y = tile["px"][1];
      uint16_t id = tile["t"];
      int idx = (level->tilesWide * (y / tileSize)) + (x / tileSize);
      bool solid =
          world->tilesetDefs[layer.def->tilesetId].tileHasTag(id, "Solid");
      SDL_RendererFlip flip = tile["f"];

      Tile layerTile(id, flip, true, solid);
      layer.tiles[idx] = layerTile;
    }
  } else if (layer.def->type == LayerType::ENTITIES) {
    for (json entityInstanceJson : layerJson["entityInstances"]) {
      Entity entity;

      entity.uid = entityUidMap[entityInstanceJson["defUid"]];
      entity.identifier = entityInstanceJson["__identifier"]; // remove this?
      entity.position = {entityInstanceJson["px"][0],
                         entityInstanceJson["px"][1]};

      for (auto field : entityInstanceJson["fieldInstances"]) {
        EntityFieldValue fieldValue;
        int defUid = field["defUid"];
        ;
        fieldValue.field =
            &world->entityDefs[entityUidMap[entity.uid]].fields[defUid];
        // switch (fieldValue.field->type) {
        // case Entity_Field_Tag::String:
        fieldValue.identifier = field["__identifier"];

        if (field["__value"].is_array()) {
          //fieldValue.value = field["__value"][0]; // @TODO: implment this 
        }
        else {
          fieldValue.value = field["__value"];
        }

        entity.fieldValues.push_back(fieldValue);
        // break;
        // case Entity_Field_Tag::Integer:
        //  @TODO: implement when needed
        // break;
        // case Entity_Field_Tag::Float:
        //  @TODO: implement when needed
        // break;
        //}
      }
      layer.entities.push_back(entity);

      std::sort(begin(layer.entities), end(layer.entities),
                [](const Entity &a, const Entity &b) {
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
    LOG_FATAL("ERROR: Failed to load the tileset texture at: %s",
              path.c_str());
  }

  std::map<std::string, std::vector<int>> tags;

  for (json tag : data["enumTags"]) {
    std::string enumName = tag["enumValueId"];
    std::vector<int> tileIds;

    for (int id : tag["tileIds"]) {
      tileIds.push_back(id);
    }

    // Sort the ids so we can do a binary search later on
    std::sort(tileIds.begin(), tileIds.end());

    tags[enumName] = tileIds;
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

  for (json fieldJson : data["fieldDefs"]) {
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

json loadProjectFile(const char *filename) {
  std::ifstream f(filename);

  if (!f.is_open()) {
    LOG_FATAL("ERROR: Failed to load the tileset file at: %s", filename);
  }

  json data = json::parse(f);

  return data;
}

World createWorld(std::string filePath) {
  std::string projectPath = filePath.substr(0, filePath.rfind('/')) + '/';
  json data = loadProjectFile(filePath.c_str());
  World world;

  world.tileSize = data["defaultGridSize"];
  world.cellSize = {
      (int)data["worldGridWidth"] / world.tileSize,
      (int)data["worldGridHeight"] / world.tileSize,
  };

  int i = 0;
  LOG_TRACE("parsing tileset defs");
  for (json tilesetDefJson : data["defs"]["tilesets"]) {
    // Skip internal icons
    if (tilesetDefJson["identifier"] == "Internal_Icons") { continue; }
    // Skip tilesets without textures
    if (tilesetDefJson["relPath"].is_null()) { continue; }

    Tileset tileset = parse_tileset(projectPath, tilesetDefJson);
    tilesetUidMap[tileset.id] = i;
    world.tilesetDefs.push_back(tileset);
    i++;
  }

  i = 0;
  LOG_TRACE("parsing entity defs");
  for (json entityDefJson : data["defs"]["entities"]) {
    EntityDef entity = parse_entity_def(entityDefJson);
    // entity.uid
    entityUidMap[entity.uid] = i;
    world.entityDefs.push_back(entity);
    i++;
  }

  i = 0;
  LOG_TRACE("parsing layer defs");
  for (json layerDefJson : data["defs"]["layers"]) {
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
    i++;
  }

  i = 0;
  LOG_TRACE("parsing levels");
  for (json levelJson : data["levels"]) {
    Level level;

    std::string identifier = levelJson["identifier"];
    level.iid = i;
    int tileSize = data["defaultGridSize"];
    level.tilesWide = (int)levelJson["pxWid"] / tileSize;
    level.tilesTall = (int)levelJson["pxHei"] / tileSize;
    level.tileSize = tileSize;

    LOG_TRACE("parsing level %s layer instances", identifier.c_str());
    for (json layerJson : levelJson["layerInstances"]) {
      level.layers.push_back(parse_layer_instance(&world, &level, layerJson));
    }

    level.cellPositionPx = {
        (int)levelJson["worldX"],
        (int)levelJson["worldY"],
    };

    level.cell = {
        (level.cellPositionPx.x / world.cellSize.x) / world.tileSize,
        (level.cellPositionPx.y / world.cellSize.y) / world.tileSize,
        level.tilesWide / world.cellSize.x,
        level.tilesTall / world.cellSize.y,
    };

    level.cell.debugInt();

    // neighbours
    for (json neighbourJson : levelJson["__neighbours"]) {
      std::string dirString = neighbourJson["dir"];
      std::string iid = neighbourJson["levelIid"];

      for (char d : dirString) {
        NeighBourDirection dir = neighbourDirectionFromLetter(d);
        level.neighbours[dir].push_back(iid);
      }
    }

    level.world = &world;
    world.levels.push_back(level);
    levelUidMap[levelJson["iid"]] = i;
    i++;
  }

  world.worldSizeInCells.x = 0;
  world.worldSizeInCells.y = 0;

  for (auto level : world.levels) {
    world.worldSizeInCells.x =
        max(world.worldSizeInCells.x, level.cell.w + level.cell.x);
    world.worldSizeInCells.y =
        max(world.worldSizeInCells.y, level.cell.h + level.cell.y);
  }

  int xMax = world.worldSizeInCells.x;
  int yMax = world.worldSizeInCells.y;

  // Allocate for the levelsByCells lookup
  world.levelsByCells = (Level **)malloc((xMax * yMax) * sizeof(Level *));

  for (Level level : world.levels) {
    for (int x = level.cell.x; x < level.cell.w + level.cell.x; x++) {
      for (int y = level.cell.y; y < level.cell.h + level.cell.y; y++) {
        int idx = (y * xMax) + x;
        world.levelsByCells[idx] = &world.levels[level.iid];
      }
    }
  }

  tilesetUidMap.clear();
  entityUidMap.clear();
  layerDefUidMap.clear();
  levelUidMap.clear();

  LOG_TRACE("Successfully loaded the world map");

  return world;
}
