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

class Tilemap
{
  public:
    Tilemap(const char* mapFile, SDL_Texture* texture) {
      int y = 0;
      std::string line;
      std::ifstream in(mapFile);

      if (!in.is_open()) {
        printf("Failed to open file \"%s\"\n", mapFile);
        return;
      }
      
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
              7 * (TILE_SIZE),
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
