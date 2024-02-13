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
#include <memory>

// Use some configuration place to specify all game objects. Maybe even glob the object directory (bad idea?)
std::unique_ptr<AbstractGameObject> instantiateGameObject(GAME_OBJECT obj) {
  std::unique_ptr<AbstractGameObject> o;
  switch (obj) {
    case GAME_OBJECT::PLAYER:
      o = std::make_unique<obj::Player>();
      break;
    case GAME_OBJECT::DOOR:
      o = std::make_unique<obj::Door>();
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
    /*
    if (tilemap != nullptr) {
      delete tilemap;
    }
    */

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
    for (TiledObject o : objects) {
      if (!gameObjects.count(o.name)) {
        printf("Warning: Object \"%s\" is not mapped in game engine\n", o.name.c_str());
        continue;
      }

      GAME_OBJECT objType = gameObjects.find(o.name)->second;

      auto object = instantiateGameObject(objType);
      object->_position = o.position;
      object->init(_renderer);
      EntityManager::Instance()->addEntity(std::move(object));
    }

    auto player = EntityManager::Instance()->getEntityByTag(OBJECT_TAG::PLAYER);
    _camera.follow(player->getRectPointer());
    loaded = true;
  }

  void update(float dt) {
    for(const auto &obj : *EntityManager::Instance()->getEntities()) {
      obj->update(dt);
    }

    _camera.update();
  }

  void draw(SDL_Renderer* renderer) {
    Rect camera = _camera.getRect();

    bg1->draw(renderer, -camera.x * 0.04);
    bg2->draw(renderer, -camera.x * 0.2);

    // Draw tiles
    for (int i = 0; i < tilemap->getTiles()->size(); i ++) {
      Tile t = tilemap->getTiles()->at(i);
      SDL_Rect r = { t.x, t.y, tilemap->getTileWidth(), tilemap->getTileHeight() };
      SDL_Rect sr = t.textureRect;

      Rect tileRect = { t.x, t.y, sr.w, sr.h };
      if (camera.hasIntersection(&tileRect)) {
        SDL_Rect dr = { t.x - camera.x, t.y - camera.y, sr.w, sr.h };
        SDL_RenderCopyEx(renderer, tilemap->getTexture(), &sr, &dr, 0, 0, SDL_FLIP_NONE);
      }
    }

    // @TODO: handle drawing some tiles after objects depending on their z-setting in the tmx-format
    // Draw objects
    for(const auto &obj : *EntityManager::Instance()->getEntities()) {
      Rect objRect = obj->getRect();
      if (objRect.hasIntersection(&camera)) {
        obj->draw(renderer, { (float)camera.x, (float)camera.y });
      }
    }
  }
};
