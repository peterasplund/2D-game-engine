#include "tilemap.h"

Tilemap::Tilemap(const char* mapFile, SDL_Renderer* renderer) {
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(mapFile);

  if (!result) {
    printf("Error: Failed to load tilemap at: %s\n", mapFile);
  }

  tileWidth = doc.child("map").attribute("tilewidth").as_int();
  tileHeight = doc.child("map").attribute("tileheight").as_int();

  // Set up tileset
  std::string tilesetSrcRel = doc.child("map").child("tileset").attribute("source").as_string();
  std::string tilesetPath = "assets/maps/";
  std::string tilesetSrcAbs = tilesetPath + tilesetSrcRel;
  _tileset.load(tilesetPath + tilesetSrcRel);

  // get object positions
  for (pugi::xml_node group : doc.child("map").children("objectgroup")) {
    if (group.attribute("name").as_string() == OBJECT_POSITIONS_NAME) {
      for (pugi::xml_node object = group.first_child(); object; object = object.next_sibling()) {
        TiledObject o;

        o.position = { object.attribute("x").as_int(), object.attribute("y").as_int() };
        o.name = object.attribute("name").as_string();

        if (!object.attribute("width").empty()) {
          o.width = object.attribute("width").as_float();
        }

        if (!object.attribute("height").empty()) {
          o.height = object.attribute("height").as_float();
        }

        for (pugi::xml_node p : object.child("properties").children("property")) {
          std::string type = p.attribute("type").as_string();
          if (type == "int") {
            o.propertiesInt.emplace(
              p.attribute("name").as_string(),
              p.attribute("value").as_int()
            );
          }
          
          if (type == "float") {
            o.propertiesFloat.emplace(
              p.attribute("name").as_string(),
              p.attribute("value").as_float()
            );
          }
          
          if (type == "string") {
            o.propertiesString.emplace(
              p.attribute("name").as_string(),
              p.attribute("value").as_string()
            );
          }
        }

        _objects.push_back(o);
      }
    }
  }

  for (auto layer : doc.child("map").children("layer")) {
    TileLayer l;

    int layerId     = layer.attribute("id").as_int();
    int layerWidth  = layer.attribute("width").as_int();
    int layerHeight = layer.attribute("height").as_int();
    std::string layerName = layer.attribute("name").as_string();

    std::stringstream dataStream(layer.child("data").child_value());
    std::stringstream lineStream;

    int y = -1;
    int x = 0;

    std::string line;
    std::string cell;

    while(std::getline(dataStream, line)) {
      x = 0;

      lineStream.clear();
      _tilesWide = line.length();
      lineStream.str(line);

      while(std::getline(lineStream, cell, ',')) {
        int cellVal = stoi(cell);
        //int idx = (y * _tilesWide) + x;
        //printf("idx: %d\n", idx);

        //l.tiles.at(idx) = cellVal;
        l.tiles.push_back(cellVal);

        x ++;
      }


      y ++;
    }

    _tilesWide = x;
    _tilesTall = y;

    _layers.push_back(l);
  }
}

// @TODO: rename to getTileRect
Rect Tilemap::getTilePosition(int layerIdx, int idx) {
  int x = 0;
  int y = 0;

  if (idx > 0) {
    x = (idx % _tilesWide) * tileWidth;
    y = (idx / _tilesWide) * tileHeight;
  }

  return { x, y, tileWidth, tileHeight };
}

int Tilemap::getIdxFromPoint(int x, int y, int layer) {
    int x_coord = x / tileWidth;
    int y_coord = y / tileHeight;

    if (x_coord > _tilesWide) {
      return -1;
    }

    if (y_coord > _tilesTall) {
      return -1;
    }

    int idx = (y_coord * _tilesWide) + x_coord;
    if (idx >= _layers.at(layer).tiles.size()) {
      return -1;
    }

    if (!_layers.at(layer).tiles.at(idx)) {
      return -1;
    }

    return idx;
}

std::vector<int> Tilemap::getIndicesWithinRect(RectF r, int layer) {
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
