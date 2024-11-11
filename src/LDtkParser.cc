#include "LDtkParser.h"
#include "assetManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

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

LDTK_Level parse_level(int tileSize, json data, std::map<int, LDTK_Tileset> tilesets) {
  LDTK_Level level;

  level.uid = data["uid"];
  level.identifier = data["identifier"];

  level.tilesWide = (int)data["pxWid"] / tileSize;
  level.tilesTall = (int)data["pxHei"] / tileSize;

  for(json layerJson : data["layerInstances"]) {
    LDTK_Level_Layer layer;
    layer.tiles.tilesetId = -1;

    std::string type = layerJson["__type"];
    layer.type = strToLayerType(type.c_str());
    layer.identifier = layerJson["__identifier"];

    if (layer.type == LayerType::INT_GRID) {
      layer.tiles.data.resize(layerJson["autoLayerTiles"].size());
      int uuid = layerJson["__tilesetDefUid"];
      layer.tiles.tilesetId = uuid;

      int maxSize = level.tilesTall * level.tilesWide;
      layer.tiles.data.resize(maxSize);

      int gridSize = (int)layerJson["__gridSize"];
      
      for(json tile : layerJson["autoLayerTiles"]) {
        int txX = tile["src"][0];
        int txY = tile["src"][1];
        int x = (int)tile["px"][0];
        int y = (int)tile["px"][1];
        int id = tile["t"];
        int idx = (level.tilesWide * (y / 16)) + (x / 16);
        TileTextureFlip flip = tile["f"];

        layer.tiles.data[idx] = { true, x, y, txX, txY, id, flip };
      }
    } else if (layer.type == LayerType::ENTITIES) {
      for(json instance : layerJson["entityInstances"]) {
        LDTK_Level_Entity entity;

        entity.identifier = instance["__identifier"];
        entity.position = { instance["px"][0], instance["px"][1] };

        layer.entities.push_back(entity);
      }
    } else if (layer.type == LayerType::TILES) {
      layer.tiles.data.resize(layerJson["intGridCsv"].size());

      int uuid = layerJson["__tilesetDefUid"];
      layer.tiles.tilesetId = uuid;
      int maxSize = level.tilesTall * level.tilesWide;
      layer.tiles.data.resize(maxSize);


      for (int i = 0; i < layerJson["gridTiles"].size(); i++) {
        json tile = layerJson["gridTiles"][i];
        int x = tile["px"][0];
        int y = tile["px"][1];

        int idx = (level.tilesWide * (y / 16)) + (x / 16);
        int txX = tile["src"][0];
        int txY = tile["src"][1];
        int id = tile["t"];
        TileTextureFlip flip = tile["f"];

        layer.tiles.data[idx] = { true, x, y, txX, txY, id, flip };
      }
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
    std::string enumName = tag["enumValueId"];
    std::vector<int> tileIds;

    for(int id : tag["tileIds"]) {
      tileIds.push_back(id);
    }

    tags.push_back({ enumName, tileIds });
  }

  LDTK_Tileset tileset;
  tileset._uid = data["uid"];
  tileset._imageWidth = imageWidth;
  tileset._tilesize = tileSize;
  tileset._texture = texture;
  tileset._tags = tags;

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
    auto layer = parse_layer(x);
    layer.project = this;
    layers.push_back(parse_layer(x));
  }

  printf("> parsing entities\n");
  for(json x : data["defs"]["entities"]) {
    auto entity = parse_entity(x);
    this->entitites[entity.identifier] = entity;
  }

  printf("> parsing tilesets\n");
  for(json x : data["defs"]["tilesets"]) {
    std::string identifier = x["identifier"];

    if (identifier != "Internal_Icons") {
      LDTK_Tileset tileset = parse_tileset(projectPath, x);
      this->tilesets.insert(std::pair(tileset._uid, tileset));
    }
  }

  // Levels
  
  printf("> parsing levels\n");

  for(json x : data["levels"]) {
    std::string identifier = x["identifier"];

    // @Temp: skip all but first level
    if (strcmp(identifier.c_str(), "Level_0") != 0) {
      continue;
    }

    printf("> > parse level %s \n", identifier.c_str());
    auto level = parse_level(defaultGridSize, x, this->tilesets);
    level.project = this;
    this->levels[identifier] = level;
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

int LDTK_Level::getIdxFromPoint(int x, int y, int layer) {
  int tilesetId = this->layers[layer].tiles.tilesetId;

  int x_coord = x / 16;
  int y_coord = y / 16;

  if (x_coord > tilesWide) {
    return -1;
  }

  if (y_coord > tilesTall) {
    return -1;
  }
  
  int idx = (y_coord * tilesWide) + x_coord;
  if (idx >= layers.at(layer).tiles.data.size()) {
    return -1;
  }

  auto tile = layers.at(layer).tiles.data.at(idx);

  if (!tile.active) {
    return -1;
  }

  return (tile.x / 16) + ((tile.y / 16) * tilesWide);
}


// Not OK?
std::vector<int> LDTK_Level::getIndicesWithinRect(RectF r, int layer) {
  int tileWidth = 16;
  int tileHeight = 16;
  std::vector<int> indices;

  for (int y = floor((r.top() - 1) / tileHeight) * tileHeight; y <= ceil(r.bottom() / tileHeight) * tileHeight; y += tileHeight) {
    for (int x = floor((r.left() - 1) / tileWidth) * tileWidth; x <= ceil(r.right() / tileWidth) * tileWidth; x += tileWidth) {
      int idx = getIdxFromPoint(x, y, layer);
      if (idx != -1) {
        indices.push_back(idx);
      }
    }
  }

  return indices;
}
