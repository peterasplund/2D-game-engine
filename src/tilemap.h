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
  v2 position;

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

    /// Returns -1 if outside the map
    int getIdxFromPoint(int x, int y) {
        int x_coord = x / tileWidth;
        int y_coord = y / tileHeight;

        // Return -1 if outide the map
        if (x_coord < 0 || y_coord < 0 ||
            x_coord > _tilesWide || y_coord > _tilesTall
            ) {
          return -1;
        }

        return (y_coord * _tilesWide) + x_coord;
    }

    Rect getTilePosition(int layerIdx, int idx) {
      int x = 0;
      int y = 0;

      if (idx > 0) {
        x = (idx % _tilesWide) * tileWidth;
        y = (idx / _tilesWide) * tileHeight;
      }

      return { x, y, tileWidth, tileHeight };
    }

    std::vector<TileLayer>* getLayers() {
      return &_layers;
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
