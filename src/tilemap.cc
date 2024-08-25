#include "tilemap.h"

Tilemap::Tilemap(int firstGID, int tilesWide, int tilesTall, Tileset tileset, std::vector<TileLayer> layers, std::vector<TiledObject> objects) {
  _firstGID = firstGID;
  _tilesWide = tilesWide;
  _tilesTall = tilesTall;
  _tileset = tileset;
  _layers = layers;
  _objects = objects;
}

Rect Tilemap::getTileRect(int layerIdx, int idx) {
  int x = 0;
  int y = 0;

  if (idx > 0) {
    x = (idx % _tilesWide) * getTileWidth();
    y = (idx / _tilesWide) * getTileHeight();
  }

  return { x, y, getTileWidth(), getTileHeight() };
}

int Tilemap::getIdxFromPoint(int x, int y, int layer) {
    int x_coord = x / getTileWidth();
    int y_coord = y / getTileHeight();

    if (x_coord > _tilesWide) {
      return -1;
    }

    if (y_coord > _tilesTall) {
      return -1;
    }

    int idx = (y_coord * _tilesWide) + x_coord;
    if (idx >= _layers.at(layer).tiles.size()) {
      return -1;
    }

    if (!_layers.at(layer).tiles.at(idx)) {
      return -1;
    }

    return idx;
}

std::vector<int> Tilemap::getIndicesWithinRect(RectF r, int layer) {
  int tileWidth = getTileWidth();
  int tileHeight = getTileHeight();
  std::vector<int> indices;

  for (int y = floor((r.top() - 1) / tileHeight) * tileHeight; y <= ceil(r.bottom() / tileHeight) * tileHeight; y += tileHeight) {
    for (int x = floor((r.left() - 1) / tileWidth) * tileWidth; x <= ceil(r.right() / tileWidth) * tileWidth; x += tileWidth) {
      int idx = getIdxFromPoint(x, y, layer);
      if (idx != -1) {
        indices.push_back(idx);
      }
    }
  }

  return indices;
}

/*
Tilemap::Tilemap(LDTK_Level ldtkLevel) {
  LDTK_Level level;

  TileLayer layer;
  Tileset tileset;

  for(auto layer : ldtkLevel.layers) {
    for(auto tile : layer.tiles.data) {
      Tile t;
    }
  }


  _layers = layers;
}
*/
