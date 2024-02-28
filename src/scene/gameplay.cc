#include "gameplay.h"

// Use some configuration place to specify all game objects. Maybe even glob the object directory (bad idea?)
std::shared_ptr<AbstractGameObject> GameplayScene::instantiateGameObject(GAME_OBJECT obj) {
  std::shared_ptr<AbstractGameObject> o;
  switch (obj) {
    case GAME_OBJECT::PLAYER:
      o = std::make_shared<obj::Player>();
      break;
    case GAME_OBJECT::DOOR:
      o = std::make_shared<obj::Door>();
      break;
  }

  return o;
}

void GameplayScene::init() {
  /*
  if (tilemap != nullptr) {
    delete tilemap;
  }
  */

  char levelName[32];
  sprintf(levelName, "assets/maps/maps/%s.tmx", _level.c_str());

  tilemap = tiled_load_map(levelName);
  EntityManager::Instance()->setTileMap(tilemap);

  _camera = camera();
  _camera.setBounds({ tilemap->getWidthInPixels(), tilemap->getHeightInPixels() });

  if (loaded) {
    return;
  }

  gameObjects = {
    { "player", GAME_OBJECT::PLAYER },
    { "door", GAME_OBJECT::DOOR },
  };

  bg1 = new Bg("assets/bgs/clouds.png", { 512, 352 });
  bg2 = new Bg("assets/bgs/town.png", { 512, 352 });

  std::vector<TiledObject> objects = tilemap->getObjects();
  for (TiledObject o : objects) {
    if (!gameObjects.count(o.name)) {
      printf("Warning: Object \"%s\" is not mapped in game engine\n", o.name.c_str());
      continue;
    }

    GAME_OBJECT objType = gameObjects.find(o.name)->second;

    auto object = instantiateGameObject(objType);
    object->_position = { (float)o.position.x, (float)o.position.y };
    object->init();
    EntityManager::Instance()->addEntity(std::move(object));
  }

  auto player = EntityManager::Instance()->getEntityByTag(OBJECT_TAG::PLAYER);
  _camera.follow(player->getRectPointer());
  _renderer->setOffsetPtr(&_camera.pos);
  loaded = true;
}

void GameplayScene::update(float dt) {
  for(const auto &obj : EntityManager::Instance()->getEntities()) {
    obj->update(dt);
  }

  _camera.update();
}

void GameplayScene::draw(Renderer* renderer) {
  RectF camera = _camera.getRect();
  v2f cameraOffset = { (float)camera.x, (float)camera.y };

  //bg1->draw(renderer, -camera.x * 0.04);
  //bg2->draw(renderer, -camera.x * 0.2);

  std::vector<TileLayer>* layers = tilemap->getLayers();
  // Draw tiles
  int layerIdx = 0;
  for (TileLayer layer : *layers) {
    int i = 0;
    for (int tile : layer.tiles) {
      if (tile == 0) {
        i ++;
        continue;
      }

      Rect tileRect = tilemap->getTileRect(layerIdx, i);

      Tileset* tileset = tilemap->getTileset();
      Rect sr = tileset->getTileTextureRect(tile);

      if (camera.hasIntersection(&tileRect)) {
        Rect dr = { 
          (int)round(tileRect.x),
          (int)round(tileRect.y),
          tileRect.w,
          tileRect.h
        };

        renderer->renderTexture(tileset->getTexture(), &sr, &dr, SDL_FLIP_NONE);
      }

      i ++;
    }
    layerIdx ++;
  }

  // @TODO: handle drawing some tiles after objects depending on their z-setting in the tmx-format
  // Draw objects
  for(const auto &obj : EntityManager::Instance()->getEntities()) {
    Rect objRect = obj->getRect();
    if (objRect.hasIntersection(&camera)) {
      obj->draw(renderer);
    }
  }
  
  DebugPrinter::Instance()->draw(renderer);
}
