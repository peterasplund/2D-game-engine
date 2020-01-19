#pragma once

#include "SDL.h"
#include <vector>
#include "animation.h"
#include "camera.h"
#include <math.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "assetManager.h"
#include "../lib/pugixml-1.10/src/pugixml.hpp"
#include "../lib/pugixml-1.10/src/pugixml.cpp"

const int TILE_SIZE = 32;

class Tilemap
{
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

      // int mapWidth = doc.child("map").attribute("width").as_int();
      // int mapHeight = doc.child("map").attribute("height").as_int();
      std::string image = doc.child("map").child("tileset").child("image").attribute("source").as_string();

      SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("maps/" + image);
      
      int tileMapWidth, tileMapHeight;
      SDL_QueryTexture(texture, NULL, NULL, &tileMapWidth, &tileMapHeight);

      int tileMapTilesWidth = floor(tileMapWidth / 32);
      int tileMapTilesHeight = floor(tileMapHeight / 32);


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

            Object o;

            o.setTexture(texture);
            // TODO: Should not be "y - 1" here. Just "y"
            o.setPosition({ (float)(x * TILE_SIZE), (float)((y - 1) * TILE_SIZE) });
            o.setSize({ TILE_SIZE, TILE_SIZE });

            int cellVal = stoi(cell);
            int tileAtX = ((cellVal - 1) % tileMapTilesWidth);
            int tileAtY = ceil(cellVal / tileMapTilesWidth);

            o.setTextureRect({
              tileAtX * (TILE_SIZE),
              tileAtY * (TILE_SIZE),
              TILE_SIZE, TILE_SIZE
            });

            if (name == "solid") {
              o.setSolid(true);
            }

            _map.push_back(o);

            x ++;
          }

          y ++;
        }
      }
    }

    ~Tilemap() {
    }

    void draw(SDL_Renderer* renderer, Camera* camera) {
      SDL_Rect c = camera->getRect();

      for (int i = 0; i < _map.size(); i ++) {
        v2 p = _map[i].getPosition();
        v2 s = _map[i].getSize();

        SDL_Rect o = { (int)p.x, (int)p.y, (int)s.x, (int)s.y };

        if (SDL_HasIntersection(&o, &c)) {
          _map[i].draw(renderer, camera->getRect());
        }
      }
    }

    std::vector<Object> getTiles() { return _map; }

  private:
    std::vector<Object> _map;
};
