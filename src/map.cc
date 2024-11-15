#include "map.h"

v2i Tileset::getTileTexturePos(int id) {
    return {
      (id * tileSize) % textureWidth,
      (id * tileSize) / textureWidth 
    };
  }

v2i Level::idxToPoint(int idx) {
  return {
    (idx % tilesWide),
    (idx / tilesWide)
  };
}

int Level::getIdxFromPoint(v2i point) {
  return (point.x / 16) + ((point.y / 16) * tilesWide);
}

v2i Layer::getTilePos(int id) {
  int i = 0;
  while (i < tiles.size()) {
    if (i == id) {
      return level->idxToPoint(i) * 16;
    }

    i ++;
  }

  return {-1, -1};
}

std::vector<int> Level::getIndicesWithinRect(Rect r) {
  int tileWidth = 16;
  int tileHeight = 16;
  std::vector<int> indices;

  for (int y = floor((r.top() - 1) / tileHeight) * tileHeight; y <= ceil(r.bottom() / tileHeight) * tileHeight; y += tileHeight) {
    for (int x = floor((r.left() - 1) / tileWidth) * tileWidth; x <= ceil(r.right() / tileWidth) * tileWidth; x += tileWidth) {
      int idx = this->getIdxFromPoint({x, y});
      if (idx != -1) {
        indices.push_back(idx);
      }
    }
  }

  return indices;
}
