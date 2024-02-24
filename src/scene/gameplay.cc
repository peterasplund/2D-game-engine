#include "gameplay.h"

bool rayVsRect(v2f& ray_origin, v2f& ray_dir, Rect* target, v2f& contact_point, v2f& contact_normal, float& t_hit_near);
bool DynamicRectVsRect(Rect* r_dynamic, v2f inVelocity, Rect& r_static, v2f& contact_point, v2f& contact_normal, float& contact_time, float dt);

// Use some configuration place to specify all game objects. Maybe even glob the object directory (bad idea?)
std::unique_ptr<AbstractGameObject> GameplayScene::instantiateGameObject(GAME_OBJECT obj) {
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

void GameplayScene::init() {
  /*
  if (tilemap != nullptr) {
    delete tilemap;
  }
  */

  char levelName[32];
  sprintf(levelName, "assets/maps/maps/%s.tmx", _level.c_str());

  tilemap = new Tilemap(levelName, _renderer);
  EntityManager::Instance()->setTileMap(tilemap);

  //_camera.setBounds({ tilemap->getWidthInPixels(), tilemap->getHeightInPixels() });

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
  // _camera.follow(player->getRectPointer());
  loaded = true;
}

void GameplayScene::update(float dt) {
  for(const auto &obj : *EntityManager::Instance()->getEntities()) {
    obj->update(dt);
  }

  // _camera.update();
}

void GameplayScene::draw(SDL_Renderer* renderer) {
  Rect camera = _camera.getRect();

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

      Rect tileRect = tilemap->getTilePosition(layerIdx, i);

      Tileset* tileset = tilemap->getTileset();
      SDL_Rect sr = tileset->getTileTextureRect(tile);

      if (camera.hasIntersection(&tileRect)) {
        //SDL_Rect dr = { tileRect.x - camera.x, tileRect.y - camera.y, tileRect.w, tileRect.h };
        SDL_Rect dr = { tileRect.x, tileRect.y, tileRect.w, tileRect.h };
        SDL_RenderCopyEx(renderer, tileset->getTexture(), &sr, &dr, 0, 0, SDL_FLIP_NONE);
      }

      i ++;
    }
    layerIdx ++;
  }

  // @TODO: handle drawing some tiles after objects depending on their z-setting in the tmx-format
  // Draw objects
  for(const auto &obj : *EntityManager::Instance()->getEntities()) {
    Rect objRect = obj->getRect();
    //if (objRect.hasIntersection(&camera)) {
      //obj->draw(renderer, { (float)camera.x, (float)camera.y });
      obj->draw(renderer, { 0.0, 0.0 });
    //}
  }
  
  DebugPrinter::Instance()->drawHitboxes(renderer, camera);
  DebugPrinter::Instance()->draw(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
}
