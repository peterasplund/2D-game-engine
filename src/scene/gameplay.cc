#include "gameplay.h"
#include "../obj/bat.h"
#include "../obj/door.h"
#include "../obj/player.h"
#include "../obj/npc.h"
#include "../debugPrinter.h"

GameState gameState;

// Use some configuration place to specify all game objects. Maybe even glob the
// object directory (bad idea?)
AbstractGameObject *GameplayScene::instantiateGameObject(GAME_OBJECT obj) {
  AbstractGameObject *o = nullptr;
  switch (obj) {
  case GAME_OBJECT::PLAYER:
    o = new obj::Player();
    break;
  case GAME_OBJECT::NPC:
    o = new obj::Npc();
    break;
  case GAME_OBJECT::DOOR:
    o = new obj::Door();
    break;
  }

  return o;
}

void GameplayScene::instantiateEntitites(Level *level) {
  AbstractGameObject *bat = new obj::Bat(); bat->init(); EntityManager::Instance()->addEntity(bat);

  for (auto layer : level->layers) {
    for (auto e : layer.entities) {
      auto entityDef = world->entityDefs[e.uid];

      if (entityDef.identifier == "NPC") {
        auto npc = instantiateGameObject(GAME_OBJECT::NPC);

        npc->_position = {(float)e.position.x, (float)e.position.y};

        npc->init();

        std::string name;
        std::string dialogue;
        for (auto field : e.fieldValues) {
          LOG_TRACE("Parsed field: %s", field.identifier.c_str());
          if (field.identifier == "name") {
            name = field.value;
          }
          if (field.identifier == "dialogue") {
            dialogue = field.value;
          }
        }

        ((obj::Npc *)npc)->setProperties(name, dialogue);

        EntityManager::Instance()->addEntity(npc);
      } else {
        // Tmp solution
        if (entityDef.identifier == "Player" && _player != nullptr) {
          continue;
        }

        auto it = gameObjects.find(entityDef.identifier);

        if (it == gameObjects.end()) {
          continue;
        }

        auto object = instantiateGameObject(it->second);
        if (object != nullptr) {
          object->_position = {(float)e.position.x, (float)e.position.y};
          object->init();

          EntityManager::Instance()->addEntity(object);
        }
      }
    }
  }
}

void GameplayScene::init() {
  dialogue = Dialogue::Instance();
  dialogue->init(_renderer);
  int numCells = 0;

  for (int i = 0; i < world->levels.size(); i++) {
    numCells += world->levels[i].cell.w * world->levels[i].cell.h;
  }

  gameState.visited.resize(numCells);
  for (int i = 0; i < numCells; i++) {
    gameState.visited[i] = false;
  }

  damageNumberSystem = DamageNumbersSystem::Instance();
  damageNumberSystem->init();

  mapHud = new MapHud(
      _renderer, world,
      {(WINDOW_WIDTH / 4) - MAP_HUD_CELL_WIDTH - (MAP_HUD_CELL_WIDTH * 5), 8});

  gameObjects = {
      {"Player", GAME_OBJECT::PLAYER},
      {"door", GAME_OBJECT::DOOR},
  };

  Level *level = &world->levels[this->_level];

  instantiateEntitites(level);

  for (auto layer : level->layers) {
    // Init tiles
    EntityManager::Instance()->setTileMap(level);
  }

  _camera = camera();
  _camera.setBounds({
      level->tilesWide * level->tileSize,
      level->tilesTall * level->tileSize,
  });

  this->levelManager = new LevelManager(world, &_camera);

  if (loaded) {
    return;
  }

  bg1 = new Bg("assets/bgs/sky.png", {480, 270});
  bg2 = new Bg("assets/bgs/houses.png", {480, 270});
  hud = new Hud();

  auto player = EntityManager::Instance()->_player;
  _player = player;
  _camera.follow(player->getRectPointer());
  _renderer->setOffsetPtr(&_camera.pos);
  loaded = true;
}

void GameplayScene::update(double dt) {
  damageNumberSystem->update(dt);

  dialogue->update();
  if (Dialogue::Instance()->isDisplayingMessage()) {
    return;
  }

  levelManager->update(dt);

  if (levelManager->isTransitioning()) {
    return;
  }

  for (const auto &obj : EntityManager::Instance()->getEntities()) {
    if (obj != nullptr) {
      obj->update(dt);
    }
  }

  _camera.update();
  levelManager->update2();

  /*
  if (pendingLevel.iid == -1) {
    gameState.visited[(playerCellPos.y * world->worldSizeInCells.x) +
                      playerCellPos.x] = true;
  }
  */

  EntityManager::Instance()->update();
}

void GameplayScene::draw(Renderer *renderer) {
  RectF camera = _camera.getRect();
  // v2f cameraOffset = { (float)camera.x, (float)camera.y };
  Level *level = &world->levels[this->_level];

  // bg1->draw(renderer->getSdlRenderer(), 0);
  // bg2->draw(renderer->getSdlRenderer(), -camera.x * 0.04);

  for (auto layer : level->layers) {
    if (layer.def->type == LayerType::ENTITIES) {
      continue;
    }

    Tileset tileset = world->tilesetDefs[layer.def->tilesetId];

    for (uint16_t i = 0; i < layer.tiles.size(); i++) {
      Tile tile = layer.tiles[i];

      if (!tile.getActive()) {
        continue;
      }

      auto texturePos = tileset.getTileTexturePos(tile.getTileId());

      v2i pos = level->idxToPoint(i);
      int tileSize = level->tileSize;

      Rect dr = {pos.x, pos.y, tileSize, tileSize};
      Rect sr = {texturePos.x, texturePos.y, tileSize, tileSize};

      renderer->renderTexture(tileset.texture, &sr, &dr,
                              (SDL_RendererFlip)tile.getFlip(), true);
    }
  }

  // @TODO: handle drawing some tiles after objects depending on their z-setting
  // in the tmx-format Draw objects
  for (const auto &obj : EntityManager::Instance()->getEntities()) {
    if (obj != nullptr) {
      Rect objRect = obj->getTextureRect();
      if (objRect.hasIntersection(&camera)) {
        obj->draw(renderer);
      }
    }
  }

  hud->draw(renderer->getSdlRenderer());

  DebugPrinter::Instance()->draw(renderer);

  mapHud->draw(_level, _player->_position);

  dialogue->draw();

  damageNumberSystem->draw(renderer);

  levelManager->drawFade(renderer);
}
