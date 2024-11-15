#include "gameplay.h"
#include "../obj/enemy.h"
#include "../obj/npc.h"

// Use some configuration place to specify all game objects. Maybe even glob the object directory (bad idea?)
std::shared_ptr<AbstractGameObject> GameplayScene::instantiateGameObject(GAME_OBJECT obj) {
  std::shared_ptr<AbstractGameObject> o = nullptr;
  switch (obj) {
    case GAME_OBJECT::PLAYER:
      o = std::make_shared<obj::Player>();
      break;
    case GAME_OBJECT::NPC:
      o = std::make_shared<obj::Npc>();
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
    EntityManager::Instance()->setTileMap(level);

    // Init entities
    for(auto e : layer.entities) {
      auto entityDef = _ldtkProject->entityDefs[e.uid];

      if (entityDef.identifier == "NPC") {
        /*
        auto npc = instantiateGameObject(GAME_OBJECT::NPC);

        npc->_position = { (float)e.position.x, (float)e.position.y };

        std::string name;
        for(auto field : e.fieldValues) {
          // @TODO: set fields on NPC
          if (field.identifier == "name") {
            printf("Got name: %s\n", field.value.c_str());
          }
          if (field.identifier == "dialogue") {
            printf("Got dialogue: %s\n", field.value.c_str());
          }
        }

        npc->init();
        EntityManager::Instance()->addEntity(npc);
        */
      }
      else {
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
  _player = player;
  _camera.follow(player->getRectPointer());
  _renderer->setOffsetPtr(&_camera.pos);
  loaded = true;
}

void GameplayScene::switchLevel(std::string level) {
  Level* lvl = &_ldtkProject->levels[level];
  EntityManager::Instance()->setTileMap(lvl);
}

void GameplayScene::update(float dt) {

  for(const auto &obj : EntityManager::Instance()->getEntities()) {
    obj->update(dt);
  }

  Level lvl = _ldtkProject->levels[this->_level];

  Rect objRect = _player->getRect();
  RectF playerRect = _player->_collidable.addBoundingBox(_player->_position);

  // @TODO: move this out and refactor
  if (playerRect.left() - 1 >= lvl.tilesWide * lvl.tileSize) {
    if (lvl.neighbours[NeighBourDirection::E].size() > 0) {
      std::string iid = lvl.neighbours[NeighBourDirection::E][0];
      printf("next level: %s\n", iid.c_str());

      this->_level = iid;
      switchLevel(this->_level);

      v2f p = _player->getPosition();
      p.x = -(playerRect.w-1);
      _player->setPosition(p);
      _player->_collidable.update(_player->_position);

      Level* lvl = &_ldtkProject->levels[this->_level];

      _camera.setBounds({
        lvl->tilesWide * lvl->tileSize,
        lvl->tilesTall * lvl->tileSize
      });
    }
  }
  else if (playerRect.right() <= 1) {
    if (lvl.neighbours[NeighBourDirection::W].size() > 0) {
      std::string iid = lvl.neighbours[NeighBourDirection::W][0];
      printf("prev level: %s\n", iid.c_str());

      this->_level = iid;
      lvl = _ldtkProject->levels[this->_level];
      switchLevel(this->_level);

      v2f p = _player->getPosition();
      p.x = (lvl.tilesWide * lvl.tileSize) - playerRect.w;
      _player->setPosition(p);
      _player->_collidable.update(_player->_position);

      Level* lvl = &_ldtkProject->levels[this->_level];

      _camera.setBounds({
        lvl->tilesWide * lvl->tileSize,
        lvl->tilesTall * lvl->tileSize
      });
    }
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

      renderer->renderTexture(tileset.texture, &sr, &dr, (SDL_RendererFlip)tile.getFlip(), true);
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
