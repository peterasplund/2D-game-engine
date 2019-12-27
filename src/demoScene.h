#pragma once
#include <SDL2/SDL.h>
#include "scene.h"
#include "player.h"
#include "tilemap.h"

char map[] = {
  'x', 'x', ' ', ' ', ' ', ' ', 'x', 'x',
  'x', ' ', ' ', ' ', ' ', ' ', ' ', 'x',
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  ' ', ' ', ' ', ' ', ' ', 'x', ' ', ' ',
  ' ', ' ', ' ', ' ', 'x', 'x', 'x', ' ',
  ' ', ' ', ' ', ' ', ' ', 'x', ' ', ' ',
  'x', ' ', ' ', ' ', ' ', ' ', ' ', 'x',
  'x', 'x', ' ', ' ', ' ', ' ', 'x', 'x',
};

class DemoScene : public Scene
{
private:
  Player* player;
  Tilemap* tilemap;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
  }

  void init() {
    player = new Player(_renderer);
    tilemap = new Tilemap(map, 8, 9);
    tilemap->printMap();
  }

  void update(float dt) {
    player->update(dt);
  }

  void draw(SDL_Renderer* renderer) {
    player->draw(renderer);
  }
};
