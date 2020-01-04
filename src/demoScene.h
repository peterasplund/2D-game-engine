#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "assetManager.h"
#include "scene.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "game/hud.h"
#include "game/gameState.h"

class DemoScene : public Scene
{
private:
  Player* player;
  Tilemap* tilemap;
  Camera* camera;
  Hud* hud;
  GameState* state;
  std::vector<Object*> entities;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
  }

  void init() {
    state = new GameState();
    player = new Player(_renderer);
    player->setPosition({ 64.0f, 64.0f });
    hud = new Hud(_renderer, state);

    camera = new Camera({ 640, 480 }, { 0, 0, 2000, 2000 });
    camera->follow(player);

    SDL_Texture* tilemapTexture = AssetManager::Instance(_renderer)->getTexture("tileset.png");
    tilemap = new Tilemap("resources/maps/demo.level", tilemapTexture);

    entities.push_back(player);
  }

  void update(float dt) {
    for (int i = 0; i < entities.size(); i ++) {
      entities[i]->update(dt);

      if (entities[i]->getId() == "player") {
        ((Player*)entities[i])->update(dt, tilemap->getTiles());
      } else {
        entities[i]->update(dt);
      }
    }

    camera->update(dt);
  }

  void draw(SDL_Renderer* renderer) {
    tilemap->draw(renderer, camera);

    for (int i = 0; i < entities.size(); i ++) {
      entities[i]->draw(renderer, camera->getRect());
    }

    hud->draw(renderer);

    SDL_RenderPresent(renderer);
  }
};
