#pragma once
#include "SDL.h"
#include <vector>
#include "scene.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "bg.h"
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
  SDL_Renderer* _renderer;

  Bg* bg1;
  Bg* bg2;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
    _renderer = renderer;
  }

  void init() {
    state = new GameState();
    player = new Player(_renderer);
    player->setPosition({ 64.0f, 64.0f });
    hud = new Hud(_renderer, state);

    camera = new Camera({ 512, 352 }, { 0, 0, 2000, 2000 });
    camera->follow(player);

    tilemap = new Tilemap("assets/maps/demo2.tmx", _renderer);

    bg1 = new Bg("bgs/clouds.png", { 512.0f, 352.0f }, _renderer);
    bg2 = new Bg("bgs/town.png", { 512.0f, 352.0f }, _renderer);

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
    bg1->draw(renderer, -camera->getRect().x * 0.04);
    bg2->draw(renderer, -camera->getRect().x * 0.2);

    tilemap->draw(renderer, camera);

    for (int i = 0; i < entities.size(); i ++) {
      entities[i]->draw(renderer, camera->getRect());
    }

    // hud->draw(renderer);

    SDL_RenderPresent(renderer);
  }
};
