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

typedef std::map<std::string, std::string> TiledLayer;

// This will be removed soon
struct TiledObject {
  std::string name;
  v2i position;

  std::optional<float> width = NULL;
  std::optional<float> height = NULL;

  std::map<std::string, int> propertiesInt;
  std::map<std::string, float> propertiesFloat;
  std::map<std::string, std::string> propertiesString;
};

// @TODO: create these in z-order
class TileLayer {
  public:
    /// ID 0 means no tile
    std::vector<int> tiles;
};

class Tilemap
{
  const std::string OBJECT_POSITIONS_NAME = "objects";
  int tileWidth, tileHeight;

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

    std::vector<TiledObject> getObjects() {
      return _objects;
    }

    bool isOutsideMap(int x, int y) {
      return (
        x < 0 || y < 0 ||
        x > _tilesWide * tileWidth || y > _tilesTall * tileHeight
      );
    }

    std::vector<int> getIndicesWithinRect(Rect r, int layer);

    /// Returns -1 if empty tile
    int getIdxFromPoint(int x, int y, int layer);

    Rect getTilePosition(int layerIdx, int idx);

    std::vector<TileLayer>* getLayers() {
      return &_layers;
    }

    TileData getTileData(int idx) {
      return _tileset.getTileData(idx);
    }

    Tileset* getTileset() {
      return &_tileset;
    }
  private:
    int _tilesWide;
    int _tilesTall;

    std::vector<TiledObject> _objects;
    Tileset _tileset;
    std::vector<TileLayer> _layers;
};
