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

// Rename this to TileObject. Not Tiled specific
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

  public:
    Tilemap(int tilesWide, int tilesTall, Tileset tileset, std::vector<TileLayer> layers, std::vector<TiledObject> objects);

    ~Tilemap() {
    }

    int getTileWidth() {
      return _tileset.getTileWidth();
    }

    int getTileHeight() {
      return _tileset.getTileHeight();
    }

    // @TODO: is this needed?
    int getWidthInPixels() {
      return _tilesWide * getTileWidth();
    }

    // @TODO: is this needed?
    int getHeightInPixels() {
      return _tilesTall * getTileHeight();
    }

    std::vector<TiledObject> getObjects() {
      return _objects;
    }

    bool isOutsideMap(int x, int y) {
      return (
        x < 0 || y < 0 ||
        x > _tilesWide * getTileWidth() || y > _tilesTall * getTileHeight()
      );
    }

    std::vector<int> getIndicesWithinRect(RectF r, int layer);

    /// Returns -1 if empty tile
    int getIdxFromPoint(int x, int y, int layer);

    Rect getTileRect(int layerIdx, int idx);

    std::vector<TileLayer>* getLayers() {
      return &_layers;
    }

    TileData* getTileData(int idx) {
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
