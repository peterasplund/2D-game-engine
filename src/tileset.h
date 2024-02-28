#pragma once

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "pugixml.hpp"
#include "assetManager.h"
#include "math.h"

enum class TileType {
  NORMAL,
  SLOPE_LEFT,
  SLOPE_RIGHT,
};

struct TileData {
  int id;
  bool solid;
  TileType type = TileType::NORMAL;

  // @TODO: try to use enum as index instead of string
  std::map<std::string, int> propertiesInt;
  std::map<std::string, float> propertiesFloat;
  std::map<std::string, bool> propertiesBool;
  std::map<std::string, std::string> propertiesString;
};

class Tileset {
  public:
    Tileset() {
    }

    bool load(std::string filename);

    Rect getTileTextureRect(int id);

    SDL_Texture* getTexture() {
      return _texture;
    }

    TileData* getTileData(int idx) {
      return &_tiles.at(idx);
    }

    int getTileWidth() {
      return _tileWidth;
    }

    int getTileHeight() {
      return _tileHeight;
    }

  private:
    SDL_Texture* _texture;
    std::vector<TileData> _tiles;
    int _tileWidth;
    int _tileHeight;
    int _tileCount;
    int _columns;
};
