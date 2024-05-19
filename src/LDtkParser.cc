#include "LDtkParser.h"
#include "assetManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json loadProjectFile(const char* filename) {
  std::ifstream f(filename);

  if (!f.is_open()) {
    printf("ERROR: Failed to load the tileset file at: %s\n", filename);
  }

  json data = json::parse(f);

  return data;
}

LDTK_Entity parse_entity(json data) {
  LDTK_Entity entity;

  entity.uid = data["uid"];
  entity.identifier = data["identifier"];

  for(json fieldJson : data["fieldDefs"]) {
    LDTK_Entity_Field field;

    field.uid = fieldJson["uid"];
    field.identifier = fieldJson["identifier"];

    if (fieldJson["__type"] == "String") {
      field.type = LDTK_Entity_Field_Tag::String;
    } else if (fieldJson["__type"] == "Integer") {
      field.type = LDTK_Entity_Field_Tag::Integer;
    }
    // @TODO: add more field types

    entity.fields.push_back(field);
  }

  return entity;
}

LDTK_Layer parse_layer(json data) {
  LDTK_Layer layer;

  layer.uid = data["uid"];
  layer.identifier = data["identifier"];

  return layer;
}

LDTK_Level parse_level(int tileSize, json data) {
  LDTK_Level level;

  level.uid = data["uid"];
  level.identifier = data["identifier"];

  level.tilesWide = (int)data["pxWid"] / tileSize;
  level.tilesTall = (int)data["pxHei"] / tileSize;

  for(json layerJson : data["layerInstances"]) {
    LDTK_Level_Layer layer;

    layer.type = layerJson["__type"];
    layer.identifier = layerJson["__identifier"];

    if (layer.type == "IntGrid") {
      // @TODO: just allocate the whole array at once
      layer.tiles.resize(layerJson["intGridCsv"].size());
      for(json x : layerJson["intGridCsv"]) {
        //@TODO: what to do about auto layer tiles?
        layer.tiles.push_back(x);
      }
    } else if (layer.type == "Entities") {
      // @TODO: parse entities
    } else if (layer.type == "Tiles") {
      layer.entities.resize(layerJson["intGridCsv"].size());
      for(json x : layerJson["entityInstances"]) {
        LDTK_Level_Entity entity;
        entity.identifier = x["__identifier"];
        // @TODO: add all fieldInstances

        layer.entities.push_back(entity);
      }
      // @TODO: just allocate the whole array at once
      /*
      layer.tiles.resize(layerJson["gridTiles"].size());
      for(json x : layerJson["tiles"]) {
        layer.tiles.push_back(x);
      }
      */
    }

    level.layers.push_back(layer);
  }

  return level;
}

LDTK_Tileset parse_tileset(std::string projectPath, json data) {
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

  std::vector<EnumTag> tags;

  for(json tag : data["enumTags"]) {
    std::string id = tag["enumValueId"];
    std::vector<int> tileIds;

    for(int id : tag["tileIds"]) {
      tileIds.push_back(id);
    }

    tags.push_back({ id, tileIds });
  }

  LDTK_Tileset tileset = LDTK_Tileset(tileSize, imageWidth, tags, texture);

  return tileset;
}

void Project::load(const std::string filePath) {
  std::string projectPath = filePath.substr(0, filePath.rfind('/')) + '/';

  json data = loadProjectFile(filePath.c_str());
  printf("> loaded project file\n");

  int defaultGridSize = data["defaultGridSize"];

  // Definitions
  
  printf("> parsing layers\n");
  for(json x : data["defs"]["layers"]) {
    layers.push_back(parse_layer(x));
  }

  printf("> parsing entities\n");
  for(json x : data["defs"]["entities"]) {
    entitites.push_back(parse_entity(x));
  }

  printf("> parsing tilesets\n");
  for(json x : data["defs"]["tilesets"]) {
    std::string identifier = x["identifier"];

    if (identifier != "Internal_Icons") {
      LDTK_Tileset tileset = parse_tileset(projectPath, x);
      tilesets.push_back(tileset);
    }
  }

  // Levels
  
  printf("> parsing levels\n");

  for(json x : data["levels"]) {
    std::string identifier = x["identifier"];
    printf("> > parse level %s \n", identifier.c_str());
    levels[identifier] = parse_level(defaultGridSize, x);
  }
}

Rect LDTK_Tileset::getTileTextureRect(int id) {
  int x = 0;
  int y = 0;

  int columns = ceil(_imageWidth / _tilesize);

  if (id > 0) {
    x = ((id % columns) - 1) * _tilesize;
    y = (id / columns) * _tilesize;
  }

  return { x, y, _tilesize, _tilesize };
}

std::vector<EnumTag*> LDTK_Tileset::getTagsByTile(int tileId) {
  std::vector<EnumTag*> results;
  for(auto tag : _tags) {
    if (std::find(tag.tileIds.begin(), tag.tileIds.end(), tileId) != tag.tileIds.end()) {
      results.push_back(&tag);
    }
  }

  return results;
}
