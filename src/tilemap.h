#pragma once

#include <optional>
#include "animation.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "assetManager.h"
#include "tileset.h"
#include "math.h"
#include "pugixml.hpp"

// This will be removed soon
struct TiledObject {
  std::string name;
  v2 position;

  std::optional<float> width = NULL;
  std::optional<float> height = NULL;

  std::map<std::string, int> propertiesInt;
  std::map<std::string, float> propertiesFloat;
  std::map<std::string, std::string> propertiesString;
};

// @TODO: create these in z-order
struct TileLayer {
  std::vector<int> tiles;
};

class Tilemap
{
  const std::string OBJECT_POSITIONS_NAME = "objects";
  int tileWidth, tileHeight;

  pugi::xml_document doc;

  // @TODO: use LDtk for maps instead so no pugixml
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

    // @TODO: is this needed?
    int getWidthInPixels() {
      return _tilesWide * tileWidth;
    }

    // @TODO: is this needed?
    int getHeightInPixels() {
      return _tilesTall * tileHeight;
    }

    SDL_Texture* getTexture() {
      return _texture;
    }

    std::vector<Tile>* getTiles() {
      return &_tiles;
    }

    std::vector<TiledObject> getObjects() {
      return _objects;
    }

    int getIdxFromPoint(int x, int y) {
        int x_coord = x / tileWidth;
        int y_coord = y / tileHeight;
        return (y_coord * _tilesWide) + x_coord;
    }

  private:
    int _tilesWide;
    int _tilesTall;

    std::vector<TiledObject> _objects;
    Tileset _tileset;
    std::vector<TileLayer> _layers;
};
