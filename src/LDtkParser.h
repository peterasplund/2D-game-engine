#pragma once

//#include "tilemap.h"
#include "math.h"
#include "SDL.h"
#include <vector>
#include <map>
#include <string>

enum class LDTK_Entity_Field_Tag { Integer, Float, String };
struct LDTK_Tileset;
struct Project;

struct LDTK_Entity_Field {
  int uid;
  std::string identifier;
  LDTK_Entity_Field_Tag type;
};

struct LDTK_Entity {
  int uid;
  std::string identifier;
  std::vector<LDTK_Entity_Field> fields;
};

struct LDTK_Layer {
  int uid;
  std::string identifier;
  Project* project;
};

struct LDTK_Level_Entity {
  std::string identifier;
  v2i position;
};

enum TileTextureFlip {
  NONE=0,
  HORIZONTAL=1,
  VERTICAL=2,
  BOTH=3,
};

struct LDTK_TileData {
  bool active;
  int x;
  int y;

  int txX;
  int txY;

  int id;

  TileTextureFlip flip;
};

struct LDTK_Level_Layer_Tiles {
  int tilesetId;
  std::vector<LDTK_TileData> data;
  LDTK_Layer* layer;
};

enum LayerType {
  ENTITIES,
  TILES,
  INT_GRID,
};

struct LDTK_Level_Layer {
  std::string identifier;
  LayerType type;
  LDTK_Level_Layer_Tiles tiles;
  std::vector<LDTK_Level_Entity> entities;
};

struct LDTK_Level_EntityFields {}; // Maybe remove this

struct LDTK_Level {
  int uid;
  std::string identifier;
  Project* project;

  int tilesWide;
  int tilesTall;

  std::vector<LDTK_Level_Layer> layers;

  // This is correct
  Rect getTileRect(int layerIdx, int idx) {
    int x = 0;
    int y = 0;

    int TILE_HEIGHT = 16;
    int TILE_WIDTH = 16;

    if (idx > 0) {
      x = (idx % tilesWide) * TILE_WIDTH;
      y = (idx / tilesWide) * TILE_HEIGHT;
    }

    return { x, y, TILE_WIDTH, TILE_HEIGHT };
  }

  int getIdxFromPoint(int x, int y, int layer);
  std::vector<int> getIndicesWithinRect(RectF r, int layer);
};

struct EnumTag {
  std::string id;
  std::vector<int> tileIds;
};

struct LDTK_Tileset {
  int _uid;
  int _tilesize;
  int _imageWidth;
  std::vector<EnumTag> _tags;
  SDL_Texture* _texture;

  std::vector<EnumTag*> getTagsByTile(int tileId);
  Rect getTileTextureRect(int id);
};

struct Project {
  void load(const std::string filePath);

  std::map<std::string, LDTK_Entity> entitites;
  std::vector<LDTK_Layer> layers;
  std::map<int, LDTK_Tileset> tilesets;
  std::map<std::string, LDTK_Level> levels;
};

Project load_ldtk_project(const char* file) ;
