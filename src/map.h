#pragma once

#include "SDL_render.h"
#include "math.h"
#include <vector>
#include <string>
#include <map>

struct Level;
struct LayerDef;

enum LayerType {
  ENTITIES,
  TILES,
  INT_GRID,
};

enum class Entity_Field_Tag { Integer, Float, String };

struct NPC {
  char* name;
  char* dialogue;
  // style npcStyle;
};

struct Entity_Field {
  int uid;
  std::string identifier;
  Entity_Field_Tag type;
  
  Entity_Field() {
    uid = -1;
  }
};

struct EntityDef {
  int uid;
  std::string identifier;
  std::map<int, Entity_Field> fields;
  v2i position;
};

struct EntityFieldValue {
  Entity_Field* field;
  std::string identifier;
  std::string value;
};

struct Entity {
  int uid;
  std::string identifier;
  v2i position;
  std::vector<EntityFieldValue> fieldValues;
};

struct TilesetTag {
  std::string id;
  std::vector<int> tileIds;
};

struct Tileset {
  int id;
  SDL_Texture *texture;
  int textureWidth;
  int tileSize;
  std::vector<TilesetTag> tags;

  v2i getTileTexturePos(int id);
};

#define TILE_SOLID 0x8000
#define TILE_FLIP_H 0x2000
#define TILE_FLIP_V 0x4000
#define TILE_ACTIVE 0x1000
#define TILE_TILE_ID_MASK 0x0FFF

struct Tile {
  uint16_t data;

  Tile() : data(0) {}
  Tile(uint16_t tileId, SDL_RendererFlip flip, bool active, bool solid) {
    data = 0;

    if (flip & SDL_FLIP_VERTICAL) {
      data |= TILE_FLIP_V;
    }
    if (flip & SDL_FLIP_HORIZONTAL) {
      data |= TILE_FLIP_H;
    }
    if (active) {
      data |= TILE_ACTIVE;
    }
    if (solid) {
      data |= TILE_SOLID;
    }

    data |= (tileId & TILE_TILE_ID_MASK);
  }

  bool getSolid() { return (data & TILE_SOLID) != 0; }
  bool getActive() { return (data & TILE_ACTIVE) != 0; }
  SDL_RendererFlip getFlip() { return (SDL_RendererFlip)((data >> 13) & 3); }
  uint16_t getTileId() { return data & TILE_TILE_ID_MASK; }
};

class Layer {
public:
  LayerDef* def;
  Level *level;

  int uid;

  std::vector<Tile> tiles;
  std::vector<Entity> entities;

  v2i getTilePos(int id);
};

enum NeighBourDirection { N, E, S, W };

NeighBourDirection neighbourDirectionFromLetter(std::string c);

struct Level {
  std::vector<Layer> layers;

  int tilesWide;
  int tilesTall;
  int tileSize;

  int getIdxFromPoint(v2i point);
  v2i idxToPoint(int idx);
  std::vector<int> getIndicesWithinRect(Rect r);

  Rect getTileRect(int tileIdx);

  // NeighBourDirection, level id (the long kind)
  std::map<NeighBourDirection, std::vector<std::string>> neighbours;
};

struct LayerDef {
  int uid;

  int tilesetId; // Maps to idx in World::tilesetDefs
  std::string identifier; // ?
  LayerType type;
};

struct World {
  std::map<std::string, Level> levels;
  std::map<int, LayerDef> layerDefs;
  std::map<int, EntityDef> entityDefs;
  std::map<int, Tileset> tilesetDefs;
};
