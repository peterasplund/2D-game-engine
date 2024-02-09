#pragma once

#include "../stdafx.h"
#include "../abstractGameobject.h"
#include "../obj/player.h"
#include "../obj/door.h"
#include "../scene.h"
#include "../tilemap.h"
#include "../bg.h"
#include "../entityManager.h"
#include <string>

// Use some configuration place to specify all game objects. Maybe even glob the object directory (bad idea?)
AbstractGameObject* instantiateGameObject(GAME_OBJECT obj) {
  AbstractGameObject* o;
  switch (obj) {
    case GAME_OBJECT::PLAYER:
      o = new obj::Player();
      break;
    case GAME_OBJECT::DOOR:
      o = new obj::Door();
      break;
  }

  return o;
}

static std::map<std::string, GAME_OBJECT> gameObjects;

class GameplayScene : public Scene
{
private:
  SDL_Renderer* _renderer;
  camera _camera;
  std::vector<AbstractGameObject*> _gameObjects;
  Bg* bg1;
  Bg* bg2;
  Tilemap* tilemap;
  std::string _level;
  bool loaded = false;
public:
  GameplayScene(SDL_Renderer* renderer, std::string level) : Scene(renderer) {
    _renderer = renderer;
    _level = level;
  }

  void init() {
    if (tilemap != nullptr) {
      delete tilemap;
    }

    char levelName[32];
    sprintf(levelName, "assets/maps/%s.tmx", _level.c_str());

    tilemap = new Tilemap(levelName, _renderer);
    EntityManager::Instance()->setTileMap(tilemap);

    _camera.setBounds({ tilemap->getWidthInPixels(), tilemap->getHeightInPixels() });

    if (loaded) {
      return;
    }

    gameObjects = {
      { "player", GAME_OBJECT::PLAYER },
      { "door", GAME_OBJECT::DOOR },
    };


    bg1 = new Bg("bgs/clouds.png", { 512.0f, 352.0f }, _renderer);
    bg2 = new Bg("bgs/town.png", { 512.0f, 352.0f }, _renderer);;


    std::vector<TiledObject> objects = tilemap->getObjects();
    obj::Player* player;
    for (TiledObject o : objects) {
      if (!gameObjects.count(o.name)) {
        printf("Warning: Object \"%s\" is not mapped in game engine\n", o.name.c_str());
        continue;
      }

      GAME_OBJECT objType = gameObjects.find(o.name)->second;


      AbstractGameObject* object = instantiateGameObject(objType);
      if (object != nullptr) {
        object->_position = o.position;
        object->init(_renderer);

        if (object->getType() == GAME_OBJECT::PLAYER) {
          player = (obj::Player*)object;
        }

        _gameObjects.push_back(object);
      }
    }

    _camera.follow(player->getRectPointer());
    loaded = true;
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

    // @TODO: handle drawing some tiles after objects depending on their z-setting in the tmx-format
    // Draw objects
    for (int i = 0; i < _gameObjects.size(); i ++) {
      SDL_Rect objRect = _gameObjects[i]->getRect();
      if (SDL_HasIntersection(&camera, &objRect)) {
        _gameObjects[i]->draw(renderer, { (float)camera.x, (float)camera.y });
      }
    }
  }
};
