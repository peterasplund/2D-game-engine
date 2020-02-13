#pragma once

#include "SDL.h"
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
#include "../lib/pugixml-1.10/src/pugixml.hpp"
#include "../lib/pugixml-1.10/src/pugixml.cpp"

enum TileType {
  NORMAL,
};

struct Tile {
  int x, y;
  SDL_Rect textureRect;
  bool solid = false;
  TileType type = TileType::NORMAL;
};

const int TILE_SIZE = 16;

class Tilemap
{
  const std::string OBJECT_POSITIONS_NAME = "objects";
    
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

      SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("maps/" + image);
      _texture = texture;
      
      int tileMapWidth, tileMapHeight;
      SDL_QueryTexture(texture, NULL, NULL, &tileMapWidth, &tileMapHeight);

      int tileMapTilesWidth = floor(tileMapWidth / TILE_SIZE);
      int tileMapTilesHeight = floor(tileMapHeight / TILE_SIZE);

      // get object positions
      for (pugi::xml_node group : doc.child("map").children("objectgroup")) {
        printf("group: %s\n", group.attribute("name").as_string());
        if (group.attribute("name").as_string() == OBJECT_POSITIONS_NAME) {
          for (pugi::xml_node object = group.first_child(); object; object = object.next_sibling()) {
            printf("\tobject: %s\n", object.attribute("name").as_string());
            std::string name = object.attribute("name").as_string();
            v2 pos = { object.attribute("x").as_float(), object.attribute("y").as_float() };

            _objects.push_back(std::pair<std::string, v2>(name, pos));
          }
        }
      }

      for (int i = 0; i < layers.size(); i ++) {
        std::string name = layers[i].first;
        std::string data = layers[i].second;

        std::stringstream  dataStream(data);
        std::stringstream lineStream;

        int y = 0, x = 0, numTilesX;
        std::string line;
        std::string cell;
        while(std::getline(dataStream, line)) {
          x = 0;

          lineStream.clear();
          lineStream.str(line);

          while(std::getline(lineStream, cell, ',')) {
            if (cell == "0") { x ++; continue; }

            int tileX = (float)(x * TILE_SIZE);
            int tileY = (float)((y - 1) * TILE_SIZE);

            int cellVal = stoi(cell);
            int tileAtX = ((cellVal - 1) % tileMapTilesWidth);
            int tileAtY = ceil(cellVal / tileMapTilesWidth);

            //printf("cellVal: %d\n", cellVal);
            if (cellVal == 34) {
              printf("x: %d y: %d\n", tileAtX, tileAtY);
              printf("width: %d\n", tileMapTilesWidth);
            }

            _tiles.push_back({
              (x * TILE_SIZE),                                                    // x position
              ((y - 1) * TILE_SIZE),                                              // y position (should not be "y - 1" here. Just "y")
              { tileAtX * TILE_SIZE, tileAtY * TILE_SIZE, TILE_SIZE, TILE_SIZE }, // Texture rect
              name == "solid",                                                    // Solid?
            });

            x ++;
          }

          y ++;
        }
      }
    }

    ~Tilemap() {
    }

    void draw(SDL_Renderer* renderer, camera* camera) {
      SDL_Rect c = Camera::getRect(camera);

      for (int i = 0; i < _tiles.size(); i ++) {
        Tile t = _tiles[i];

        SDL_Rect o = { t.x, t.y, TILE_SIZE, TILE_SIZE };

        if (SDL_HasIntersection(&o, &c)) {
          SDL_Rect r = { t.x - c.x, t.y - c.y, TILE_SIZE, TILE_SIZE };
          SDL_RenderCopy(renderer, _texture, &t.textureRect, &r);
        }
      }
    }

    std::vector<Tile>* getTiles() {
      return &_tiles;
    }

    std::vector<std::pair<std::string, v2>> getObjects() {
      return _objects;
    }

  private:
    std::vector<Tile> _tiles;
    std::vector<std::pair<std::string, v2>> _objects;
    SDL_Texture* _texture;
};
