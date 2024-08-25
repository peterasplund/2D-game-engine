#pragma once

//#include "tilemap.h"
#include "math.h"
#include "SDL.h"
#include <vector>
#include <map>
#include <string>

enum class LDTK_Entity_Field_Tag { Integer, Float, String };
struct LDTK_Tileset;

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

  int tilesWide;
  int tilesTall;

  std::vector<LDTK_Level_Layer> layers;
};

struct EnumTag {
  std::string id;
  std::vector<int> tileIds;
};

struct LDTK_Tileset {
  /*
  LDTK_Tileset(int uid, int tileSize, int imageWidth, std::vector<EnumTag> tags, SDL_Texture* texture) {
    _uid = uid;
    _tilesize = tileSize;
    _imageWidth = imageWidth;
    _tags = tags;
    _texture = texture;
  }
  */
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
