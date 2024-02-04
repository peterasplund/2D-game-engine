#pragma once
#include "abstractGameobject.h"
#include "player.h"
#include "stdafx.h"
#include <vector>
#include "scene.h"
#include "tilemap.h"
#include "bg.h"
#include "systems/collisionSystem.h"

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
  CollisionHandler* _collisionHandler;
  //Hud* hud;
  //GameState* state;
  camera _camera;
  std::vector<AbstractGameObject*> _gameObjects;
  Bg* bg1;
  Bg* bg2;
  Tilemap* tilemap;
  std::vector<std::vector<bool>> solidTiles;
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
    //_collisionHandler = new CollisionHandler();
    solidTiles = tilemap->getSolidTiles();
    //tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);

    // tilemap->addTilesToRegistry();

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

    Camera::follow(&_camera, player->getRectPointer());

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
    // initCollisionSystem();
    _collisionHandler->init(&_gameObjects);
  }

  void update(float dt) {
    _collisionHandler->beforeUpdate(&_gameObjects, &solidTiles);

    for (int i = 0; i < _gameObjects.size(); i ++) {
      _gameObjects[i]->update(dt);
    }
    _camera.update();
    /*
    animationSystem(dt);
    cameraSystem();

    characterControllerSystem(InputHandler::Instance(), _renderer);
    gravitySystem(dt);
    */
    _collisionHandler->afterUpdate(&_gameObjects);
    /*
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
    //SDL_Rect camera = { 50, 50, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
    SDL_Rect pRect = _gameObjects[0]->getRect();
    SDL_Rect camera = Camera::getRect(&_camera);

    bg1->draw(renderer, -camera.x * 0.04);
    bg2->draw(renderer, -camera.x * 0.2);

    //printf("x: %d\ty: %d\tw: %d\th: %d\n", camera.x, camera.y, camera.w, camera.h);
    std::vector<Tile>* tiles = tilemap->getTiles();
    for (int i = 0; i < tiles->size(); i ++) {
      Tile t = tiles->at(i);
      SDL_Rect r = { t.x, t.y, tilemap->getTileWidth(), tilemap->getTileHeight() };
      SDL_Rect sr = t.textureRect;
      //printf("x: %d\ty: %d\tw: %d\th: %d\n", dr.x, dr.y, dr.w, dr.h);

      SDL_Rect tileRect = { t.x, t.y, sr.w, sr.h };
      if (SDL_HasIntersection(&camera, &tileRect)) {
        SDL_Rect dr = { t.x - camera.x, t.y - camera.y, sr.w, sr.h };
        SDL_RenderCopyEx(renderer, tilemap->getTexture(), &sr, &dr, 0, 0, SDL_FLIP_NONE);
      }
    }

    for (int i = 0; i < _gameObjects.size(); i ++) {
      SDL_Rect objRect = _gameObjects[i]->getRect();
      if (SDL_HasIntersection(&camera, &objRect)) {
        _gameObjects[i]->draw(renderer, { (float)camera.x, (float)camera.y });
      }
    }

    //renderableSystem(renderer);
    //debugSystem(_renderer);

    //hud->draw(renderer);

    SDL_RenderPresent(renderer);

  }
};
