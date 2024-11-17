#include "map-hud.h"

MapHud::MapHud(Renderer* renderer, World* world, v2i position) {
  _renderer = renderer;
  _position = position;

  int x, y, w, h;

  _worldCellWidth = world->worldCellWidth;
  _worldCellHeight = world->worldCellHeight;

  int tilesInCellX = world->worldCellWidth;
  int tilesInCellY = world->worldCellHeight;

  for(auto level : world->levels) {
    MapCell cell;
    
    cell.iid = level.first;
    cell.type = CellType::Normal;
    cell.position = level.second.cellPosition;
    cell.size = {
      level.second.tilesWide / tilesInCellX,
      level.second.tilesTall / tilesInCellY,
    };

    if (level.second.neighbours[NeighBourDirection::N].size() == 0) { cell.north = BorderValue::Wall; }
    if (level.second.neighbours[NeighBourDirection::E].size() == 0) { cell.east = BorderValue::Wall; }
    if (level.second.neighbours[NeighBourDirection::S].size() == 0) { cell.south = BorderValue::Wall; }
    if (level.second.neighbours[NeighBourDirection::W].size() == 0) { cell.west = BorderValue::Wall; }

    _cells.push_back(cell);
  }
}

void MapHud::drawCell(MapCell cell, Marker marker, v2i playerTilePosition) {
  Rect rect = {
    _position.x + (cell.position.x * MAP_HUD_CELL_WIDTH),
    _position.y + (cell.position.y * MAP_HUD_CELL_HEIGHT),
    MAP_HUD_CELL_WIDTH * cell.size.x,
    MAP_HUD_CELL_HEIGHT * cell.size.y,
  };

  Rect innerRect = { rect.x + 1, rect.y + 1, rect.w - 1, rect.h - 1 };

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


  // Draw Whole cell (BG)
  _renderer->renderRectFilled(&rect, false);

  switch (marker) {
    case Marker::Player:
      _renderer->setColor(80, 80, 240, 255);
      int playerMarkerSize = 4;

      v2i currentCell = {
        (playerTilePosition.x / _worldCellWidth),
        (playerTilePosition.y / _worldCellHeight),
      };

      Rect playerMarker = { 
        ((innerRect.x + (MAP_HUD_CELL_WIDTH - 2) / 2) - (playerMarkerSize / 2)) + (currentCell.x * MAP_HUD_CELL_WIDTH),
        ((innerRect.y + (MAP_HUD_CELL_HEIGHT - 2) / 2) - (playerMarkerSize / 2)) + (currentCell.y * MAP_HUD_CELL_HEIGHT),
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

void MapHud::draw(std::string currentLevel, v2i playerTilePosition) {
  for(auto cell : _cells) {
    Marker marker = currentLevel ==  cell.iid ? Marker::Player : Marker::None;

    drawCell(cell, marker, playerTilePosition);
  }
}
