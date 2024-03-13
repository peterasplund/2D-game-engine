#include "tileset.h"

Tileset::Tileset(
    SDL_Texture* texture, 
    std::map<int, TileData> tiles,
    int tileSize,
    int tileCount,
    int columns
) {
  _texture = texture;
  _tiles = tiles;
  _tileCount = tileCount;
  _tileWidth = tileSize;
  _tileHeight = tileSize;
  _columns = columns;
}

bool Tileset::load(std::string filename) {
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename.c_str());

  if (!result) {
    printf("ERROR: Failed to load the tileset file at: %s\n", filename.c_str());
    return false;
  }

  pugi::xml_node tilesetNode = doc.child("tileset");

  if (!tilesetNode) {
    printf("ERROR: No tileset node\n");
    return false;
  }

  pugi::xml_node imageNode = tilesetNode.child("image");
  
  if (!imageNode) {
    printf("ERROR: No image node\n");
    return false;
  }

  _tileWidth = tilesetNode.attribute("tilewidth").as_int();
  _tileHeight = tilesetNode.attribute("tileheight").as_int();
  _tileCount = tilesetNode.attribute("tileCount").as_int();
  _columns = tilesetNode.attribute("columns").as_int();

  std::string absPath = "assets/maps/tilesets/";
  std::string imgSrc = imageNode.attribute("source").as_string();
  _texture = AssetManager::Instance()->getTexture(absPath + imgSrc);

  if (_texture == nullptr) {
    return false;
  }

  _tiles = std::map<int, TileData>();

  for (pugi::xml_node tileNode : tilesetNode.children("tile")) {
    TileData tile;
    tile.id = tileNode.attribute("id").as_int();

    // Add all properties to tile
    for (pugi::xml_node p : tileNode.child("properties").children("property")) {
      std::string type = p.attribute("type").as_string();
      if (type == "int") {
        tile.propertiesInt.emplace(
          p.attribute("name").as_string(),
          p.attribute("value").as_int()
        );
      }
      
      if (type == "float") {
        tile.propertiesFloat.emplace(
          p.attribute("name").as_string(),
          p.attribute("value").as_float()
        );
      }
      
      if (type == "string") {
        tile.propertiesString.emplace(
          p.attribute("name").as_string(),
          p.attribute("value").as_string()
        );
      }

      if (type == "bool") {
        std::string name = p.attribute("name").as_string();

        if (name.compare("solid") == 0) {
          tile.solid = p.attribute("value").as_bool();
        }

        tile.propertiesBool.emplace(
          name,
          p.attribute("value").as_bool()
        );
      }
    }

    _tiles.emplace(tile.id, tile);
  }

  return true;
}

Rect Tileset::getTileTextureRect(int id) {
  int x = 0;
  int y = 0;

  if (id > 0) {
    x = ((id % _columns) - 1) * _tileWidth;
    y = (id / _columns) * _tileHeight;
  }

  return { x, y, _tileWidth, _tileHeight };
}
