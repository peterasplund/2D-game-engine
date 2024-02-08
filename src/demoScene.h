#pragma once

#include "stdafx.h"
#include "abstractGameobject.h"
#include "player.h"
#include "box.h"
#include "scene.h"
#include "tilemap.h"
#include "bg.h"
#include "entityManager.h"

// Use some configuration place to specify all game objects. Maybe even glob the object directory (bad idea?)
AbstractGameObject* instantiateGameObject(GAME_OBJECT obj) {
  switch (obj) {
    case GAME_OBJECT::PLAYER:
      Player* p = new Player();
      return p;
  }
  return nullptr;
}


static std::map<std::string, GAME_OBJECT> gameObjects;

class DemoScene : public Scene
{
private:
  SDL_Renderer* _renderer;
  //Hud* hud;
  //GameState* state;
  camera _camera;
  std::vector<AbstractGameObject*> _gameObjects;
  Bg* bg1;
  Bg* bg2;
  Tilemap* tilemap;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
    _renderer = renderer;
  }

  void init() {
    gameObjects = {
      { "player", GAME_OBJECT::PLAYER },
    };

    //state = new GameState();
    //hud = new Hud(_renderer, state);
    tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);
    EntityManager::Instance()->setTileMap(tilemap);
    //tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);
    
    Player* player = new Player();
    player->init(_renderer);

    //Box* box = new Box();
    //box->init(_renderer);

    _gameObjects.push_back(player);
    //_gameObjects.push_back(box);

    _camera.setBounds({ tilemap->getWidthInPixels(), tilemap->getHeightInPixels() });
    //_camera.follow(box->getRectPointer());

    bg1 = new Bg("bgs/clouds.png", { 512.0f, 352.0f }, _renderer);
    bg2 = new Bg("bgs/town.png", { 512.0f, 352.0f }, _renderer);


    std::vector<TiledObject> objects = tilemap->getObjects();
    for (TiledObject o : objects) {
      GAME_OBJECT objType = gameObjects.find(o.name)->second;

      AbstractGameObject* object = instantiateGameObject(objType);

      /*
      if (object != nullptr) {
        _gameObjects.push_back(object);
      }
      */
    }

     _camera.follow(player->getRectPointer());
  }

  void update(float dt) {
    for (int i = 0; i < _gameObjects.size(); i ++) {
      _gameObjects[i]->update(dt);
    }
    _camera.update();
  }

  void draw(SDL_Renderer* renderer) {
    SDL_Rect pRect = _gameObjects[0]->getRect();
    SDL_Rect camera = _camera.getRect();

    bg1->draw(renderer, -camera.x * 0.04);
    bg2->draw(renderer, -camera.x * 0.2);

    // Draw objects
    for (int i = 0; i < _gameObjects.size(); i ++) {
      SDL_Rect objRect = _gameObjects[i]->getRect();
      if (SDL_HasIntersection(&camera, &objRect)) {
        _gameObjects[i]->draw(renderer, { (float)camera.x, (float)camera.y });
      }
    }

    // Draw tiles
    for (int i = 0; i < tilemap->getTiles()->size(); i ++) {
      Tile t = tilemap->getTiles()->at(i);
      SDL_Rect r = { t.x, t.y, tilemap->getTileWidth(), tilemap->getTileHeight() };
      SDL_Rect sr = t.textureRect;

      SDL_Rect tileRect = { t.x, t.y, sr.w, sr.h };
      if (SDL_HasIntersection(&camera, &tileRect)) {
        SDL_Rect dr = { t.x - camera.x, t.y - camera.y, sr.w, sr.h };
        SDL_RenderCopyEx(renderer, tilemap->getTexture(), &sr, &dr, 0, 0, SDL_FLIP_NONE);
      }
    }

    SDL_RenderPresent(renderer);

  }
};
