#include "ldtk.h"
#include "assetManager.h"
#include <fstream>

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

  LayerDef* layerDef = &world->layerDefs[layerDefId];
  
  layer.def = layerDef;
  
  // layer.tilesetId = -1;

  // layer.type = strToLayerType(type.c_str());
  // layer.identifier = layerJson["__identifier"];

  if (layerDef->type == LayerType::INT_GRID || layerDef->type == LayerType::TILES) {
    layer.tiles.resize(level->tilesTall * level->tilesWide);
    layerDef->tilesetId = layerJson["__tilesetDefUid"];
  }

  if (layerDef->type == LayerType::INT_GRID) {
    /*
    for(json tile : layerJson["autoLayerTiles"]) {
      int txX = tile["src"][0];
      int txY = tile["src"][1];
      int x = (int)tile["px"][0];
      int y = (int)tile["px"][1];
      int id = tile["t"];
      int idx = (level->tilesWide * (y / 16)) + (x / 16);
      SDL_RendererFlip flip = tile["f"];

      layer.tiles[idx] = Tile(id, flip, layer.identifier == "Collision");
    }
    */
  } 
  else if (layerDef->type == LayerType::TILES) {
    for (int i = 0; i < layerJson["gridTiles"].size(); i++) {
      json tile = layerJson["gridTiles"][i];
      int x = tile["px"][0];
      int y = tile["px"][1];

      int idx = (level->tilesWide * (y / 16)) + (x / 16);
      int txX = tile["src"][0];
      int txY = tile["src"][1];
      uint16_t id = tile["t"];
      SDL_RendererFlip flip = tile["f"];
      Tile layerTile( id, flip, true, layerDef->identifier == "Collision");

      layer.tiles[idx] = layerTile;
    }
  }
  else if (layerDef->type == LayerType::ENTITIES) {
    for(json instance : layerJson["entityInstances"]) {
      Entity entity;

      entity.identifier = instance["__identifier"];
      entity.position = { instance["px"][0], instance["px"][1] };

      layer.entities.push_back(entity);
    }
  }

  return layer;
}

Tileset parse_tileset(std::string projectPath, json data) {
  // https://ldtk.io/json/#ldtk-TilesetDefJson

  int tileSize = data["tileGridSize"];
  int spacing = data["spacing"];
  int imageWidth = data["pxWid"];
  std::string relPath = data["relPath"];

  std::string path = projectPath + relPath;
  auto texture = AssetManager::Instance()->getTexture(path);

  if (texture == nullptr) {
    printf("ERROR: Failed to load the tileset texture at: %s\n", path.c_str());
  }

  std::vector<TilesetTag> tags;

  for(json tag : data["enumTags"]) {
    std::string enumName = tag["enumValueId"];
    std::vector<int> tileIds;

    for(int id : tag["tileIds"]) {
      tileIds.push_back(id);
    }

    tags.push_back({ enumName, tileIds });
  }

  Tileset tileset;
  tileset.id = data["uid"];
  tileset.textureWidth = imageWidth;
  tileset.tileSize = tileSize;
  tileset.texture = texture;
  tileset.tags = tags;

  return tileset;
}

Entity parse_entity(json data) {
  Entity entity;

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

    entity.fields.push_back(field);
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


  printf("> parsing tileset defs\n");
  for(json x : data["defs"]["tilesets"]) {
    std::string identifier = x["identifier"];

    if (identifier != "Internal_Icons") {
      Tileset tileset = parse_tileset(projectPath, x);
      world.tilesetDefs[tileset.id] = tileset;
    }
  }

  printf("> parsing layer defs\n");
  for(json x : data["defs"]["layers"]) {
    std::string type = x["type"];

    LayerDef layerDef;
    layerDef.uid = x["uid"];
    layerDef.identifier = x["identifier"];
    layerDef.tilesetId = x["tilesetDefUid"].is_null() ? -1 : (int)x["tilesetDefUid"];
    layerDef.type = strToLayerType(type.c_str());

    // @TODO: implement tiles and auto tiles

    world.layerDefs[x["uid"]] = layerDef;
  }

  printf("> parsing entity defs\n");
  for(json x : data["defs"]["entities"]) {
    auto entity = parse_entity(x);
    world.entityDefs[entity.identifier] = entity;
  }

  printf("> parsing levels\n");
  for(json x : data["levels"]) {
    Level level;

    std::string identifier = x["identifier"];
    int tileSize = data["defaultGridSize"];
    level.tilesWide = (int)x["pxWid"] / tileSize;
    level.tilesTall = (int)x["pxHei"] / tileSize;
    level.tileSize = tileSize;

    // @Temp: skip all but first level
    if (strcmp(identifier.c_str(), "Level_0") != 0) {
      continue;
    }

    printf("> parsing level %s layer instances\n", identifier.c_str());
    /*
    for(json layerJson : x["layerInstances"]) {
      Layer layer;
      layer.uid = layerJson["layerDefUid"];
      exit(0);
      
      int tilesetUid = layerJson["__tilesetDefUid"];
      if (tmpTilesetMap.count(tilesetUid) == 0) {
        printf("Couldn't get tileset by uid %d", tilesetUid);
        exit(1);
      }

      layer.tilesetId = tmpTilesetMap[tilesetUid];
      level.layers.push_back(parse_layer_instance(&world, &level, x));
    }
    */
  
    world.levels[identifier] = level;
  }

  return world;
}
