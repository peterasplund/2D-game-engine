#include "gameplay.h"
#include "../obj/bat.h"
#include "../obj/door.h"
#include "../obj/player.h"
#include "../obj/npc.h"
#include "../debugPrinter.h"
#include "../engine/settings.h"

GameState gameState;

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

  int windowWidth = gameSettings().windowWidth;

  mapHud = new MapHud(
      _renderer, world,
      {(windowWidth) - MAP_HUD_CELL_WIDTH - (MAP_HUD_CELL_WIDTH * 5), 8});

  camera* _camera = &EntityManager::Instance()->_camera;
  this->levelManager = new LevelManager(world);
  Level *level = &world->levels[this->levelManager->_level];
  if (level == nullptr) {
    LOG_FATAL("Level was nullptr. Exit.");
  }

  levelManager->setLevel(this->levelManager->_level);

  _camera->setBounds({
      level->tilesWide * level->tileSize,
      level->tilesTall * level->tileSize,
  });

  if (loaded) {
    return;
  }

  bg1 = new Bg("assets/bgs/sky.png", {480, 270});
  bg2 = new Bg("assets/bgs/houses.png", {480, 270});
  hud = new Hud();

  auto player = EntityManager::Instance()->_player;
  _player = player;
  _camera->follow(player->getRectPointer());
  _renderer->setOffsetPtr(&_camera->pos);
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

  // Update visited on map
  v2i playerCellPos = world->getCellByPx(_player->getPosition(),  levelManager->_level);
  gameState.visited[(playerCellPos.y * world->worldSizeInCells.x) +
                    playerCellPos.x] = true;

  EntityManager::Instance()->_camera.update();
  EntityManager::Instance()->update();
}

void GameplayScene::draw(Renderer *renderer) {
  RectF camera = EntityManager::Instance()->_camera.getRect();
  Level *level = &world->levels[this->levelManager->_level];

  // bg1->draw(renderer->getSdlRenderer(), 0);
  // bg2->draw(renderer->getSdlRenderer(), -camera.x * 0.04);

  for (auto layer : level->layers) {
    if (layer.def->type == LayerType::ENTITIES) {
      for (const auto &obj : EntityManager::Instance()->getEntities()) {
        if (obj != nullptr) {
          Rect objRect = obj->getTextureRect();
          if (objRect.hasIntersection(&camera)) {
            obj->draw(renderer);
          }
        }
      }

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


  hud->draw(renderer->getSdlRenderer());

  DebugPrinter::Instance()->draw(renderer);

  mapHud->draw(this->levelManager->_level, _player->_position);

  dialogue->draw();

  damageNumberSystem->draw(renderer);

  levelManager->drawFade(renderer);
}
