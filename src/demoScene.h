#pragma once
#include "abstractGameobject.h"
#include "player.h"
#include "stdafx.h"
#include <vector>
#include "scene.h"
#include "tilemap.h"
#include "bg.h"
/*
#include "game/hud.h"
#include "game/gameState.h"

#include "components/position.h"
#include "components/velocity.h"
#include "components/renderable.h"
#include "components/camera.h"
#include "components/batSpawner.h"
#include "systems/renderableSystem.h"
#include "systems/velocitySystem.h"
#include "systems/animationSystem.h"
#include "systems/characterControllerSystem.h"
#include "systems/cameraSystem.h"
#include "systems/collisionSystem.h"
#include "systems/gravitySystem.h"
#include "systems/debugSystem.h"
#include "systems/lifetimeSystem.h"
#include "systems/batSpawnerSystem.h"
#include "systems/skeletonAISystem.h"
#include "systems/batSystem.h"
#include "objects/player.h"
#include "objects/bat.h"
#include "objects/camera.h"
#include "objects/movingPlatform.h"
#include "objects/skeleton.h"
#include "objects/batSpawner.h"
*/

class DemoScene : public Scene
{
private:
  SDL_Renderer* _renderer;
  //Hud* hud;
  //GameState* state;
  std::vector<AbstractGameObject*> _gameObjects;
  Bg* bg1;
  Bg* bg2;
  Tilemap* tilemap;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
    _renderer = renderer;
  }

  void init() {
    // bind events
    //destroyOnTouchSolidBind();
    //hurtOnTouchHurtableBind();

    //state = new GameState();
    //hud = new Hud(_renderer, state);
    tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);
    //tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);

    tilemap->addTilesToRegistry();

    /*
    PrefabMap prefabs = {
      { "player", &createPlayer },
      //{ "bat", &createBat },
      { "batSpawner", &createBatSpawner },
      { "skeleton", &createSkeleton }
    };
    */
    
    Player* player = new Player();
    player->init(_renderer);
    _gameObjects.push_back(player);

    bg1 = new Bg("bgs/clouds.png", { 512.0f, 352.0f }, _renderer);
    bg2 = new Bg("bgs/town.png", { 512.0f, 352.0f }, _renderer);


    /*
    std::vector<TiledObject> objects = tilemap->getObjects();
    for (TiledObject o : objects) {
      if (prefabs.find(o.name) == prefabs.end()) {
        continue;
      }

      CreateFunction fn = prefabs.at(o.name);

      if (fn != nullptr) {
        fn(_renderer, o);
      }
    }

    createMovingPlatform(_renderer, { 64, 192 }, { .5f, 0 });
    */

    //createCamera(tilemap);

    // Lag bats
    // for (int i = 0; i < 2000; i++) { createBat(_renderer, { 0, 0 }, { 0, 10.0f }); }

    // @TODO: set prevRect to rect in collidable constructor instead.
    //initCollisionSystem();
  }

  void update(float dt) {
    for (int i = 0; i < _gameObjects.size(); i ++) {
      _gameObjects[i]->update(dt);
    }
    /*
    animationSystem(dt);
    cameraSystem();

    characterControllerSystem(InputHandler::Instance(), _renderer);
    gravitySystem(dt);
    setCollisionSystemPrevCollisionBox();
    velocitySystem(dt);
    lifetimeSystem(dt);
    batSystem();

    batSpawnerSystem(_renderer);
    skeletonAISystem();

    // Run collisions last
    collisionSystem();
    */
  }

  void draw(SDL_Renderer* renderer) {
    /*
    auto view = registry.view<camera>();
    camera c;
    SDL_Rect cr;

    for (auto entity : view) {
      c = view.get<camera>(entity);
      cr = Camera::getRect(&c);
    }
    */
    float cameraX = 0.0f;

    bg1->draw(renderer, -cameraX * 0.04);
    bg2->draw(renderer, -cameraX * 0.2);

    std::vector<Tile>* tiles = tilemap->getTiles();
    for (int i = 0; i < tiles->size(); i ++) {
      Tile t = tiles->at(i);
      SDL_Rect r = { t.x, t.y, tilemap->getTileWidth(), tilemap->getTileHeight() };
      SDL_Rect sr = t.textureRect;
      SDL_Rect dr = { t.x, t.y, sr.w, sr.h };

      SDL_RenderCopyEx(renderer, tilemap->getTexture(), &sr, &dr, 0, 0, SDL_FLIP_NONE);
    }

    for (int i = 0; i < _gameObjects.size(); i ++) {
      _gameObjects[i]->draw(renderer, { cameraX, 0.0f });
    }

    //renderableSystem(renderer);
    //debugSystem(_renderer);

    //hud->draw(renderer);

    SDL_RenderPresent(renderer);

  }
};
