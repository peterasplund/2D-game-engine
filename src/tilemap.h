#pragma once

#include "stdafx.h"
#include <optional>
#include <vector>
#include "animation.h"
#include <math.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "assetManager.h"
#include "components/camera.h"
#include "components/collidable.h"
#include "components/solid.h"
#include "components/renderable.h"

struct TiledObject {
  std::string name;
  v2 position;

  std::optional<float> width = NULL;
  std::optional<float> height = NULL;

  std::map<std::string, int> propertiesInt;
  std::map<std::string, float> propertiesFloat;
  std::map<std::string, std::string> propertiesString;
};

enum TileType {
  NORMAL,
};

struct Tile {
  int x, y;
  SDL_Rect textureRect;
  bool solid = false;
  TileType type = TileType::NORMAL;
};

class Tilemap
{
  const std::string OBJECT_POSITIONS_NAME = "objects";
  int tileWidth, tileHeight;

  pugi::xml_document doc;

  std::vector<std::pair<std::string, std::string>> getLayers(pugi::xml_parse_result result) {
    std::vector<std::pair<std::string, std::string>> layers;

    pugi::xml_node layerNodes = doc.child("map");

    for (pugi::xml_node node = layerNodes.first_child(); node; node = node.next_sibling()) {
      if ((std::string)node.name() != "layer") { continue; }

      std::string data = node.child("data").child_value();
      layers.push_back(std::pair<std::string, std::string>(node.attribute("name").as_string(), data.c_str()));
    }

    return layers;
  }

  public:
    Tilemap(const char* mapFile, SDL_Renderer* renderer) {
      pugi::xml_parse_result result = doc.load_file(mapFile);

      std::vector<std::pair<std::string, std::string>> layers = getLayers(result);

      std::string image = doc.child("map").child("tileset").child("image").attribute("source").as_string();
      tileWidth = doc.child("map").child("tileset").attribute("tilewidth").as_int();
      tileHeight = doc.child("map").child("tileset").attribute("tileheight").as_int();

      SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("maps/" + image);
      _texture = texture;
      
      int textureWidth, textureHeight;
      SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

      _tilesInTextureX = floor(textureWidth / tileWidth);
      _tilesInTextureY = floor(textureHeight / tileHeight);

      // get object positions
      for (pugi::xml_node group : doc.child("map").children("objectgroup")) {
        printf("group: %s\n", group.attribute("name").as_string());
        if (group.attribute("name").as_string() == OBJECT_POSITIONS_NAME) {
          for (pugi::xml_node object = group.first_child(); object; object = object.next_sibling()) {
            printf("\tobject: %s\n", object.attribute("name").as_string());

            TiledObject t;

            t.position = { object.attribute("x").as_float(), object.attribute("y").as_float() };
            t.name = object.attribute("name").as_string();

            if (!object.attribute("width").empty()) {
              t.width = object.attribute("width").as_float();
            }

            if (!object.attribute("height").empty()) {
              t.height = object.attribute("height").as_float();
            }

            for (pugi::xml_node p : object.child("properties").children("property")) {
              std::string type = p.attribute("type").as_string();
              if (type == "int") {
                t.propertiesInt.emplace(
                  p.attribute("name").as_string(),
                  p.attribute("value").as_int()
                );
              }
              
              if (type == "float") {
                t.propertiesFloat.emplace(
                  p.attribute("name").as_string(),
                  p.attribute("value").as_float()
                );
              }
              
              if (type == "string") {
                t.propertiesString.emplace(
                  p.attribute("name").as_string(),
                  p.attribute("value").as_string()
                );
              }
            }

            _objects.push_back(t);
          }
        }
      }

      for (int i = 0; i < layers.size(); i ++) {
        std::string name = layers[i].first;
        std::string data = layers[i].second;

        std::stringstream  dataStream(data);
        std::stringstream lineStream;

        int y = -1;
        int x = 0;

        std::string line;
        std::string cell;

        while(std::getline(dataStream, line)) {
          x = 0;

          lineStream.clear();
          lineStream.str(line);

          while(std::getline(lineStream, cell, ',')) {
            if (cell == "0") { x ++; continue; }

            int tileX = (float)(x * tileWidth);
            int tileY = (float)(y * tileHeight);

            int cellVal = stoi(cell);
            int tileAtX = ((cellVal - 1) % _tilesInTextureX);
            int tileAtY = ceil(cellVal / _tilesInTextureX);

            _tiles.push_back({
              (x * tileWidth),                                                      // x position
              ((y) * tileHeight),                                                   // y position
              { tileAtX * tileWidth, tileAtY * tileHeight, tileWidth, tileHeight }, // Texture rect
              name == "solid",                                                      // Solid?
            });

            x ++;
          }

          y ++;
        }

        _tilesWide = x;
        _tilesTall = y;
      }
    }

    ~Tilemap() {
    }

    int getTileWidth() {
      return tileWidth;
    }

    int getTileHeight() {
      return tileHeight;
    }

    int getWidthInPixels() {
      return _tilesWide * tileWidth;
    }

    int getHeightInPixels() {
      return _tilesTall * tileHeight;
    }

    SDL_Texture* getTexture() {
      return _texture;
    }

    void addTilesToRegistry() {
      for (Tile t : _tiles) {
        SDL_Rect r = { t.x, t.y, tileWidth, tileHeight };
        SDL_Rect collisionR = { 0, 0, r.w, r.h };
        auto entity = registry.create();
        registry.emplace<position>(entity, (float)t.x, (float)t.y);
        registry.emplace<renderable>(entity, _texture, t.textureRect);

        if (t.solid) {
          registry.emplace<collidable>(entity, collisionR);
          registry.emplace<solid>(entity, true);
        }
      }
    }

    std::vector<Tile>* getTiles() {
      return &_tiles;
    }

    std::vector<TiledObject> getObjects() {
      return _objects;
    }

  private:
    std::vector<Tile> _tiles;
    int _tilesWide;
    int _tilesTall;
    int _tilesInTextureX;
    int _tilesInTextureY;

    std::vector<TiledObject> _objects;
    SDL_Texture* _texture;
};
