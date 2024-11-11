/*
#pragma once

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "pugixml.hpp"
#include "assetManager.h"
#include "math.h"
#include "LDtkParser.h"

enum class TileType {
  NORMAL,
  SLOPE_LEFT,
  SLOPE_RIGHT,
};

struct TileData {
  int id;
  bool solid;
  TileType type = TileType::NORMAL;
  LDTK_Layer* layer;
};

class Tileset {
  public:
    Tileset() {};
    Tileset(
        SDL_Texture* texture, 
        std::map<int, TileData> tiles,
        int tileSize,
        int tileCount,
        int columns
    ) {
      _texture = texture;
      _tiles = tiles;
      _tileHeight = tileSize;
      _tileWidth = tileSize;
      _tileCount = tileCount;
      _columns = columns;
    }

    Rect getTileTextureRect(int id);

    SDL_Texture* getTexture() {
      return _texture;
    }

    TileData* getTileData(int id) {
      return &_tiles[id];
    }

    int getTileWidth() {
      return _tileWidth;
    }

    int getTileHeight() {
      return _tileHeight;
    }

    TileData* tileAt(int idx) {
      return &_tiles[idx];
    }

  private:
    SDL_Texture* _texture;
    std::map<int, TileData> _tiles;
    int _tileWidth;
    int _tileHeight;
    int _tileCount;
    int _columns;
};
*/
