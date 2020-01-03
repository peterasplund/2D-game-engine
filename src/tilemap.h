#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "animation.h"
#include "camera.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>

const int TILE_SIZE = 32;
const int ZOOM = 1;

class Tilemap
{
  public:
    Tilemap(const char* mapFile, SDL_Texture* texture) {
      std::ifstream in(mapFile);

      if (!in.is_open()) {
        printf("Failed to open file \"%s\"\n", mapFile);
        return;
      }
      
      int y = 0;
      std::string line;
      while (!in.eof()) {
        getline(in, line);

        for (int x = 0; x < line.size(); x ++) {
          char current = line[x];

          if (current != ' ') {
            Object o;

            o.setTexture(texture);
            o.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
            o.setSize({ TILE_SIZE, TILE_SIZE });
            o.setTextureRect({
              0 * (TILE_SIZE),
              8 * (TILE_SIZE),
              TILE_SIZE, TILE_SIZE
            });

            if (current == 'x') {
              o.setSolid(true);
            }

            _map.push_back(o);
          }
        }

        y ++;
      }

      in.close();
    }

    ~Tilemap() {
    }

    void draw(SDL_Renderer* renderer, Camera* camera) {
      for (int i = 0; i < _map.size(); i ++) {
        _map[i].draw(renderer, camera->getRect());
      }
    }

  private:
    std::vector<Object> _map;
};
