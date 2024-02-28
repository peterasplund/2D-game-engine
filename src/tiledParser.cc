#include "tilemapParser.h"

Tilemap* tiled_load_map(const char* mapFile) {
  const std::string OBJECT_POSITIONS_NAME = "objects";

  int tileHeight, tileWidth, tilesTall, tilesWide;
  std::vector<TiledObject> objects;
  std::vector<TileLayer> layers;

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
  Tileset tileset;
  tileset.load(tilesetPath + tilesetSrcRel);

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

        objects.push_back(o);
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
      tilesWide = line.length();
      lineStream.str(line);

      while(std::getline(lineStream, cell, ',')) {
        int cellVal = stoi(cell);
        l.tiles.push_back(cellVal);

        x ++;
      }

      y ++;
    }

    tilesWide = x;
    tilesTall = y;

    layers.push_back(l);
  }

  return new Tilemap(
    tilesWide,
    tilesTall,
    std::move(tileset),
    std::move(layers),
    std::move(objects)
  );
}
