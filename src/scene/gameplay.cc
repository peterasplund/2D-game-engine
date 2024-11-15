#include "gameplay.h"
#include "../obj/enemy.h"

// Use some configuration place to specify all game objects. Maybe even glob the object directory (bad idea?)
std::shared_ptr<AbstractGameObject> GameplayScene::instantiateGameObject(GAME_OBJECT obj) {
  std::shared_ptr<AbstractGameObject> o = nullptr;
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
  gameObjects = {
    { "Player", GAME_OBJECT::PLAYER },
    { "door", GAME_OBJECT::DOOR },
  };

  for(auto layer : _ldtkProject->levels[_level].layers) {
    // Init tiles
    for(auto t : layer.tiles) {
      EntityManager::Instance()->setTileMap(&_ldtkProject->levels[_level]);
    }

    // Init entities
    for(auto e : layer.entities) {
      auto entity = _ldtkProject->entityDefs[e.identifier];

      auto it = gameObjects.find(entity.identifier);

      if (it == gameObjects.end()) {
        continue;
      }

      auto object = instantiateGameObject(it->second);
      if (object != nullptr) {
        object->_position = { (float)e.position.x, (float)e.position.y };
        object->init();

        EntityManager::Instance()->addEntity(object);
      }
    }
  }

  _camera = camera();
  _camera.setBounds({ 
    _ldtkProject->levels[_level].tilesWide * 16,
    _ldtkProject->levels[_level].tilesTall * 16,
  });

  if (loaded) {
    return;
  }

  bg1 = new Bg("assets/bgs/sky.png", { 480, 270 });
  bg2 = new Bg("assets/bgs/houses.png", { 480, 270 });
  hud = new Hud();

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

  //bg1->draw(renderer->getSdlRenderer(), 0);
  //bg2->draw(renderer->getSdlRenderer(), -camera.x * 0.04);

  int tilesWide = _ldtkProject->levels[_level].tilesWide;

  for(auto layer : _ldtkProject->levels[_level].layers) {
    if (layer.def->type == LayerType::ENTITIES) {
      continue;
    }

    Tileset tileset = _ldtkProject->tilesetDefs[layer.def->tilesetId];

    for(int i = 0; i < layer.tiles.size(); i ++) {
      Tile tile = layer.tiles[i];

      if (!tile.getActive()) {
        continue;
      }

      auto texturePos = tileset.getTileTexturePos(tile.getTilesetId());

      v2i pos = _ldtkProject->levels[_level].idxToPoint(i);
      Rect dr = { pos.x, pos.y, 16, 16 };
      Rect sr = { texturePos.x, texturePos.y, 16, 16 };

      renderer->renderTexture(tileset.texture, &sr, &dr, (SDL_RendererFlip)tile.getFlip(), false);
    }
  }

  // @TODO: handle drawing some tiles after objects depending on their z-setting in the tmx-format
  // Draw objects
  for(const auto &obj : EntityManager::Instance()->getEntities()) {
    Rect objRect = obj->getTextureRect();
    //if (objRect.hasIntersection(&camera)) {
      obj->draw(renderer);
    //}
  }

  hud->draw(renderer->getSdlRenderer());
  
  DebugPrinter::Instance()->draw(renderer);
}
