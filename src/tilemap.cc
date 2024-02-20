#include "tilemap.h"

std::vector<Layer> Tilemap::getLayers(pugi::xml_parse_result result) {
  std::vector<Layer> layers;

  pugi::xml_node layerNodes = doc.child("map");

  for (pugi::xml_node node = layerNodes.first_child(); node; node = node.next_sibling()) {
    if ((std::string)node.name() != "layer") { continue; }

    std::string data = node.child("data").child_value();
    layers.push_back(Layer(node.attribute("name").as_string(), data.c_str()));
  }

  return layers;
}

Tilemap::Tilemap(const char* mapFile, SDL_Renderer* renderer) {
  pugi::xml_parse_result result = doc.load_file(mapFile);

  std::vector<Layer> layers = getLayers(result);

  std::string image = doc.child("map").child("tileset").child("image").attribute("source").as_string();
  tileWidth = doc.child("map").child("tileset").attribute("tilewidth").as_int();
  tileHeight = doc.child("map").child("tileset").attribute("tileheight").as_int();

  SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("maps/" + image);
  _texture = texture;
  
  int textureWidth, textureHeight;
  SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

  _tilesInTextureX = floor(textureWidth / tileWidth);
  _tilesInTextureY = floor(textureHeight / tileHeight);

  // Prepare vector
  _tiles.resize(TILE_MAX_LIMIT);

  // get object positions
  for (pugi::xml_node group : doc.child("map").children("objectgroup")) {
    printf("group: %s\n", group.attribute("name").as_string());
    if (group.attribute("name").as_string() == OBJECT_POSITIONS_NAME) {
      for (pugi::xml_node object = group.first_child(); object; object = object.next_sibling()) {
        printf("\tobject: %s\n", object.attribute("name").as_string());

        TiledObject t;

        t.position = { object.attribute("x").as_float(), object.attribute("y").as_float() };
        t.name = object.attribute("name").as_string();

        if (!object.attribute("width").empty()) {
          t.width = object.attribute("width").as_float();
        }

        if (!object.attribute("height").empty()) {
          t.height = object.attribute("height").as_float();
        }

        for (pugi::xml_node p : object.child("properties").children("property")) {
          std::string type = p.attribute("type").as_string();
          if (type == "int") {
            t.propertiesInt.emplace(
              p.attribute("name").as_string(),
              p.attribute("value").as_int()
            );
          }
          
          if (type == "float") {
            t.propertiesFloat.emplace(
              p.attribute("name").as_string(),
              p.attribute("value").as_float()
            );
          }
          
          if (type == "string") {
            t.propertiesString.emplace(
              p.attribute("name").as_string(),
              p.attribute("value").as_string()
            );
          }
        }

        _objects.push_back(t);
      }
    }
  }

  for (int i = 0; i < layers.size(); i ++) {
    std::string name = layers[i].first;
    std::string data = layers[i].second;

    std::stringstream  dataStream(data);
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
        if (cell == "0") { x ++; continue; }

        //std::vector<bool> row(x, y);

        int tileX = (float)(x * tileWidth);
        int tileY = (float)(y * tileHeight);
        // _solidTiles[y][x] = name == "solid";

        int cellVal = stoi(cell);
        int tileAtX = ((cellVal - 1) % _tilesInTextureX);
        int tileAtY = ceil(cellVal / _tilesInTextureX);

        int idx = (y * _tilesWide) + x;

        _tiles.at(idx) = {
          true,
          (x * tileWidth),                                                      // x position
          ((y) * tileHeight),                                                   // y position
          { tileAtX * tileWidth, tileAtY * tileHeight, tileWidth, tileHeight }, // Texture rect
          name == "solid",                                                      // Solid?
          TileType::SLOPE_LEFT,
        };

        x ++;
      }

      y ++;
    }

    _tilesWide = x;
    _tilesTall = y;
  }
}
