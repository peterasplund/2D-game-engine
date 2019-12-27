#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "animation.h"
#include <map>
#include <string>

class Tilemap
{
  public:
    Tilemap(char map[], int mapWidth, int mapHeight) {
      _tiles = std::vector<std::vector<char>>(mapHeight);

      for (int y = 0; y < mapHeight; y ++) {
        _tiles[y] = std::vector<char>(mapWidth);

        for (int x = 0; x < mapWidth; x ++) {
          int index = (y * mapWidth) + x;

          /*
          if (sizeof(&map) / sizeof(map[0]) < index) {
            printf("Error: Tried to load tilemap wider or taller than specified");
            SDL_Quit();
          }
          */

          _tiles[y][x] = map[index];
        }
      }
    }

    ~Tilemap() {
    }

    std::vector<std::vector<char>> getMap() {
      return _tiles;
    }

    char getTileAt(int x, int y) {
      return _tiles[y][x];
    }

    void printMap() {
      for (int y = 0; y < _tiles.size(); y ++) {
        for (int x = 0; x < _tiles[y].size(); x ++) {
          if (_tiles[y][x] == ' ') {
            printf("%c", '.');
          } else {
            printf("%c", _tiles[y][x]);
          }
        }
        printf("\n");
      }
    }

  private:
    std::vector<std::vector<char>> _tiles;

};
