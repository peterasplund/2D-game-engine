#pragma once

//#include "tilemap.h"
#include "math.h"
#include "SDL.h"
#include <vector>
#include <map>
#include <string>

enum class LDTK_Entity_Field_Tag { Integer, Float, String };

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
};

struct LDTK_Level_Layer {
  std::string identifier;
  std::string type;
  std::vector<int> tiles;
  std::vector<LDTK_Level_Entity> entities;
};

struct LDTK_Level_EntityFields {}; // Maybe remove this

struct LDTK_Level {
  int uid;
  std::string identifier;

  int tilesWide;
  int tilesTall;

  std::vector<LDTK_Level_Layer> layers;
  std::vector<LDTK_Level_Entity> entities;
};

struct EnumTag {
  std::string id;
  std::vector<int> tileIds;
};

struct LDTK_Tileset {
  LDTK_Tileset(int tileSize, int imageWidth, std::vector<EnumTag> tags, SDL_Texture* texture) {
    _tilesize = tileSize;
    _imageWidth = imageWidth;
    _tags = tags;
    _texture = texture;
  }

  int _tilesize;
  int _imageWidth;
  std::vector<EnumTag> _tags;
  SDL_Texture* _texture;

  std::vector<EnumTag*> getTagsByTile(int tileId);
  Rect getTileTextureRect(int id);
};

struct Project {
  void load(const std::string filePath);

  std::vector<LDTK_Entity> entitites;
  std::vector<LDTK_Layer> layers;
  std::vector<LDTK_Tileset> tilesets;
  std::map<std::string, LDTK_Level> levels;
};

Project load_ldtk_project(const char* file) ;
