#pragma once

#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "assetManager.h"

enum class TileType {
  NORMAL,
  SLOPE_LEFT,
  SLOPE_RIGHT,
};

struct TileData {
  int x, y;
  bool solid;
  TileType type = TileType::NORMAL;
};

class Tileset {
  public:
    Tileset() {
    }

    bool init(char* filename) {
      _texture = AssetManager::Instance()->getTexture(filename);

      if (_texture == nullptr) {
        return false;
      }

      return true;
    }

  private:
    SDL_Texture* _texture;
    std::vector<TileData> _tiles;
    int _tileWidth;
    int _tileHeight;
};
