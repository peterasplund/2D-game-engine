#pragma once

#include <optional>
#include "animation.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "assetManager.h"
#include "math.h"
#include "pugixml.hpp"

#define TILE_MAX_LIMIT 1024

typedef std::pair<std::string, std::string> Layer;

struct TilePosition {
  int x;
  int y;
};

struct TiledObject {
  std::string name;
  v2 position;

  std::optional<float> width = NULL;
  std::optional<float> height = NULL;

  std::map<std::string, int> propertiesInt;
  std::map<std::string, float> propertiesFloat;
  std::map<std::string, std::string> propertiesString;
};

enum TileType {
  NORMAL,
};

struct Tile {
  int x, y;
  SDL_Rect textureRect;
  bool solid = false;
  TileType type = TileType::NORMAL;
};

class Tilemap
{
  const std::string OBJECT_POSITIONS_NAME = "objects";
  int tileWidth, tileHeight;

  std::vector<std::vector<bool>> _solidTiles;

  pugi::xml_document doc;

  std::vector<Layer> getLayers(pugi::xml_parse_result result);

  public:
    Tilemap(const char* mapFile, SDL_Renderer* renderer);

    ~Tilemap() {
    }

    int getTileWidth() {
      return tileWidth;
    }

    int getTileHeight() {
      return tileHeight;
    }

    int getWidthInPixels() {
      return _tilesWide * tileWidth;
    }

    int getHeightInPixels() {
      return _tilesTall * tileHeight;
    }

    SDL_Texture* getTexture() {
      return _texture;
    }

    std::vector<std::vector<bool>> getSolidTiles() {
      return _solidTiles;
    }

    std::vector<Tile>* getTiles() {
      return &_tiles;
    }

    std::vector<TiledObject> getObjects() {
      return _objects;
    }

  private:
    std::vector<Tile> _tiles;
    int _tilesWide;
    int _tilesTall;
    int _tilesInTextureX;
    int _tilesInTextureY;

    std::vector<TiledObject> _objects;
    SDL_Texture* _texture;
};
