#include "map.h"

NeighBourDirection neighbourDirectionFromLetter(char c) {
  switch (c) {
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
      printf("Error: \"%c\" not a direction\n", c);
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

v2i Layer::getTilePos(uint32_t id) {
  int tileSize = this->level->tileSize;

  uint32_t i = 0;
  while (i < tiles.size()) {
    if (i <= level->tilesWide * level->tilesTall) {
      if (i == id) {
        return level->idxToPoint(i) * tileSize;
      }
    }

    i ++;
  }

  return {-1, -1};
}

void Level::getIndicesWithinRect(Rect r, std::vector<int>& out) {
  int tileWidth = this->tileSize;
  int tileHeight = this->tileSize;
  int maxTileId = tilesWide * tilesTall;

  int yStart = floor((r.top() - 1) / tileHeight) * tileHeight;
  int xStart = floor((r.left() - 1) / tileWidth) * tileWidth;

  for (int y = yStart; y <= ceil(r.bottom() / tileHeight) * tileHeight; y += tileHeight) {
    for (int x = xStart; x <= ceil(r.right() / tileWidth) * tileWidth; x += tileWidth) {
      if (x < tileWidth * tilesWide && y < tileWidth * tilesTall) {
        int idx = this->getIdxFromPoint({x, y});
        if (idx != -1 && idx >= 0 && idx <= maxTileId) {
          out.push_back(idx);
        }
      }
    }
  }
}

Rect Level::getTileRect(int tileIdx) {
  if (tileIdx > (tilesWide * tilesTall)) {
    printf("Error: Checking tiles out of bounds\n");
    exit(1);
  }

  v2i point = idxToPoint(tileIdx);
  return {
    point.x,
    point.y,
    this->tileSize,
    this->tileSize
  };
}

int Level::getTileSize() {
  return this->world->tileSize;
}

Level* World::getLevelByCell(v2i cell) {
  int idx = (cell.y * worldSizeInCells.x) + cell.x;
  return levelsByCells[idx];
}

v2i World::getCellByPx(v2i px, int currentLevelId) {
  Level* lvl = &levels[currentLevelId];
  return ((px / tileSize) / cellSize) + lvl->getPosition();
}
