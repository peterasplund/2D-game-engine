#pragma once
#include <SDL2/SDL.h>
#include "assetManager.h"
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

  SDL_Texture* tilemapTexture;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
  }

  void init() {
    player = new Player(_renderer);

    tilemapTexture = AssetManager::Instance(_renderer)->getTexture("tileset.png");

    tilemap = new Tilemap(map, 8, 9);
  }

  void update(float dt) {
    player->update(dt);
  }

  void draw(SDL_Renderer* renderer) {
    tilemap->draw(renderer, tilemapTexture);
    player->draw(renderer);
    SDL_RenderPresent(renderer);
  }
};
