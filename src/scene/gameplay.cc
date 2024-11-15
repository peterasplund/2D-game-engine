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

  Level* level = &_ldtkProject->levels[this->_level];

  for(auto layer : level->layers) {
    // Init tiles
    for(auto t : layer.tiles) {
      EntityManager::Instance()->setTileMap(level);
    }

    // Init entities
    for(auto e : layer.entities) {
      auto entityDef = _ldtkProject->entityDefs[e.uid];

      auto it = gameObjects.find(entityDef.identifier);

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
    level->tilesWide * level->tileSize,
    level->tilesTall * level->tileSize,
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
  Level* level = &_ldtkProject->levels[this->_level];

  //bg1->draw(renderer->getSdlRenderer(), 0);
  //bg2->draw(renderer->getSdlRenderer(), -camera.x * 0.04);

  for(auto layer : level->layers) {
    if (layer.def->type == LayerType::ENTITIES) {
      continue;
    }

    Tileset tileset = _ldtkProject->tilesetDefs[layer.def->tilesetId];

    for(int i = 0; i < layer.tiles.size(); i ++) {
      Tile tile = layer.tiles[i];

      if (!tile.getActive()) {
        continue;
      }

      auto texturePos = tileset.getTileTexturePos(tile.getTileId());

      v2i pos = level->idxToPoint(i);
      int tileSize = level->tileSize;

      Rect dr = { pos.x, pos.y, tileSize, tileSize };
      Rect sr = { texturePos.x, texturePos.y, tileSize, tileSize };

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
