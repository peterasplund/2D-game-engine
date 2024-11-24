#include "map-hud.h"
#include "globals.h"

extern GameState gameState;

MapHud::MapHud(Renderer* renderer, World* world, v2i position) {

  _renderer = renderer;
  _position = position;

  int x, y, w, h;

  _worldCellWidth = world->cellSize.x;
  _worldCellHeight = world->cellSize.y;

  _mapSizeInCells = world->worldSizeInCells;

  int tilesInCellX = world->cellSize.x;
  int tilesInCellY = world->cellSize.y;

  for(auto level : world->levels) {
    int levelWidth = level.cell.w;
    int levelHeight = level.cell.h;

    for (int x = 0; x < levelWidth; x++) {
      for (int y = 0; y < levelHeight; y++) {
        MapCell cell;

        cell.iid = level.iid;
        cell.type = CellType::Normal;
        cell.position = { level.cell.x, level.cell.y };
        cell.position.x += x;
        cell.position.y += y;

        if (level.neighbours[NeighBourDirection::N].size() == 0) { cell.north = BorderValue::Wall; }
        if (level.neighbours[NeighBourDirection::E].size() == 0) { cell.east = BorderValue::Wall; }
        if (level.neighbours[NeighBourDirection::S].size() == 0) { cell.south = BorderValue::Wall; }
        if (level.neighbours[NeighBourDirection::W].size() == 0) { cell.west = BorderValue::Wall; }

        _cells.push_back(cell);
      }
    }
  }

  this->world = world;
}

void MapHud::drawCell(MapCell cell, Marker marker, v2i playerCellPos) {
  Rect rect = {
    _position.x + (cell.position.x * MAP_HUD_CELL_WIDTH),
    _position.y + (cell.position.y * MAP_HUD_CELL_HEIGHT),
    MAP_HUD_CELL_WIDTH,
    MAP_HUD_CELL_HEIGHT,
  };

  Rect innerRect = { rect.x + 1, rect.y + 1, rect.w - 1, rect.h - 1 };

  int x = cell.position.x;
  int y = cell.position.y;

  _renderer->setColor(40, 40, 40, 255);
  if (gameState.visited[(cell.position.y * _mapSizeInCells.x) + cell.position.x]) {
    switch (cell.type) {
      case CellType::Normal:
        _renderer->setColor(20, 20, 240, 255);
        break;
      case CellType::Save:
        _renderer->setColor(240, 20, 20, 255);
        break;
      case CellType::Teleport:
        _renderer->setColor(240, 240, 20, 255);
        break;
    }
  }


  // Draw Whole cell (BG)
  _renderer->renderRectFilled(&rect, false);

  switch (marker) {
    case Marker::Player:
      _renderer->setColor(80, 80, 240, 255);
      int playerMarkerSize = 4;

      Rect playerMarker = { 
        ((innerRect.x + (MAP_HUD_CELL_WIDTH - 2) / 2) - (playerMarkerSize / 2)) ,
        ((innerRect.y + (MAP_HUD_CELL_HEIGHT - 2) / 2) - (playerMarkerSize / 2)),

        playerMarkerSize,
        playerMarkerSize
      };
      _renderer->renderRectFilled(&playerMarker, false);
      break;
  }

  _renderer->setColor(255, 255, 255, 255);

  // Draw walls (Outlines)
  if (cell.north == BorderValue::Wall) {
    Rect wall = { rect.x, rect.y, rect.w, 1 };
    _renderer->renderRectFilled(&wall, false);
  }
  else {
    Rect wall1 = { innerRect.x,           rect.top(), 2, 1 };
    Rect wall2 = { innerRect.right() - 2, rect.top(), 2, 1 };
    _renderer->renderRectFilled(&wall1, false);
    _renderer->renderRectFilled(&wall2, false);
  }

  if (cell.south == BorderValue::Wall) {
    Rect wall = { rect.x, rect.bottom() - 1, rect.w, 1 };
    _renderer->renderRectFilled(&wall, false);
  }
  else {
    Rect wall1 = { innerRect.x, rect.bottom() - 1, 2, 1 };
    Rect wall2 = { innerRect.right() - 2, rect.bottom() - 1, 2, 1 };
    _renderer->renderRectFilled(&wall1, false);
    _renderer->renderRectFilled(&wall2, false);
  }

  if (cell.east == BorderValue::Wall) {
    Rect wall = { rect.right() - 1, rect.y, 1, rect.h };
    _renderer->renderRectFilled(&wall, false);
  }
  else {
    Rect wall1 = { innerRect.right() - 1, rect.top(), 1, 2 };
    Rect wall2 = { innerRect.right() - 1, rect.bottom() - 2, 1, 2 };
    _renderer->renderRectFilled(&wall1, false);
    _renderer->renderRectFilled(&wall2, false);
  }

  if (cell.west == BorderValue::Wall) {
    Rect wall = { rect.x, rect.y, 1, rect.h };
    _renderer->renderRectFilled(&wall, false);
  }
  else {
    Rect wall1 = { innerRect.left() - 1, rect.top(),        1, 2 };
    Rect wall2 = { innerRect.left() - 1, rect.bottom() - 2, 1, 2 };
    _renderer->renderRectFilled(&wall1, false);
    _renderer->renderRectFilled(&wall2, false);
  }
}

void MapHud::draw(int currentLevel, v2i playerPosition) {
  v2i playerCellPos = world->getCellByPx(playerPosition,  currentLevel);

  for(auto cell : _cells) {
    Marker marker = Marker::None;

    if (playerCellPos == cell.position) {
      marker = Marker::Player;
      //printf("x: %d, y: %d\n", cell.position.x, cell.position.y);
    }

    drawCell(cell, marker, playerCellPos);
  }
}
