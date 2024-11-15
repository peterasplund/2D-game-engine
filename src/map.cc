#include "map.h"

NeighBourDirection neighbourDirectionFromLetter(std::string c) {
  switch (c[0]) {
    case 'n':
      return NeighBourDirection::N;
    case 'e':
      return NeighBourDirection::E;
    case 's':
      return NeighBourDirection::S;
    case 'w':
      return NeighBourDirection::W;
    case 'o':
      printf("Error: One or more  level is overlapping\n");
      exit(1);
    default:
      printf("Error: \"%c\" not a direction\n", c[0]);
      exit(1);
  }
}

v2i Tileset::getTileTexturePos(int id) {
    int widthInTiles = textureWidth / tileSize;
    int x = id % widthInTiles;
    int y = id / widthInTiles;

    return {
      x * tileSize,
      y * tileSize
    };
  }

v2i Level::idxToPoint(int idx) {
  return {
    (idx % tilesWide) * tileSize,
    (idx / tilesWide) * tileSize
  };
}

int Level::getIdxFromPoint(v2i point) {
  return (point.x / tileSize) + ((point.y / tileSize) * tilesWide);
}

v2i Layer::getTilePos(int id) {
  int tileSize = this->level->tileSize;

  int i = 0;
  while (i < tiles.size()) {
    if (i == id) {
      return level->idxToPoint(i) * tileSize;
    }

    i ++;
  }

  return {-1, -1};
}

std::vector<int> Level::getIndicesWithinRect(Rect r) {
  int tileWidth = this->tileSize;
  int tileHeight = this->tileSize;
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

Rect Level::getTileRect(int tileIdx) {
  v2i point = idxToPoint(tileIdx);
  return {
    point.x,
    point.y,
    this->tileSize,
    this->tileSize
  };
}