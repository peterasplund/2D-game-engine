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

int min(int a, int b);
int max(int a, int b);

void GameplayScene::drawFade() {
  if  (pendingLevel.iid != "") {
    int fade = max(min(transitionTimer, 255), 0);
    //int fade = transitionTimer < 0 ? 0 : transitionTimer > 255 ? 255 : 0;
    Rect fadeRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    _renderer->setColor(0, 0, 0, fade);
    _renderer->renderRectFilled(&fadeRect, false);
  }
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

void GameplayScene::switchLevel(LevelTransition level) {
  this->_level = level.iid;
  Level* lvl = &_ldtkProject->levels[this->_level];
  //EntityManager::Instance()->clearAllButConstantEntities();
  EntityManager::Instance()->setTileMap(lvl);

  _player->setPosition(pendingLevel.playerPosition);
  _player->_collidable.update(_player->_position);

  _camera.setBounds({
    lvl->tilesWide * lvl->tileSize,
    lvl->tilesTall * lvl->tileSize
  });
}

void GameplayScene::update(float dt) {
  if (pendingLevel.iid != "") {
    if (!isFadingIn) {
      transitionTimer += LEVEL_FADE_SPEED;
      if (transitionTimer >= 255) {
        isFadingIn = true;
        switchLevel(pendingLevel);
      }
    }
    else {
      transitionTimer -= LEVEL_FADE_SPEED;
      if (transitionTimer <= 0) {
        isFadingIn = false;
        pendingLevel = { "", {0,0} };
      }
    }

    // update fade values
    transitionTimer ++;

    if (!isFadingIn) {
      return;
    }
  }

  for(const auto &obj : EntityManager::Instance()->getEntities()) {
    obj->update(dt);
  }

  Level lvl = _ldtkProject->levels[this->_level];
  RectF playerRect = _player->_collidable.addBoundingBox(_player->_position);

  char dir = '-';
  std::string* iid = nullptr;
  v2f newPlayerPos = _player->getPosition();

  if (playerRect.left() - 1 >= lvl.tilesWide * lvl.tileSize) {
    if (lvl.neighbours[NeighBourDirection::E].size() > 0) {
      dir = 'e';
      iid = &lvl.neighbours[NeighBourDirection::E][0];

      newPlayerPos.x = -(playerRect.w-1);
    }
  }
  else if (playerRect.right() <= 1) {
    if (lvl.neighbours[NeighBourDirection::W].size() > 0) {
      dir = 'w';
      iid = &lvl.neighbours[NeighBourDirection::W][0];

      int tilesWide = _ldtkProject->levels[*iid].tilesWide;

      newPlayerPos.x = (tilesWide * lvl.tileSize) - playerRect.w;
    }
  }
  else if (playerRect.top() >= lvl.tilesTall * lvl.tileSize) {
    if (lvl.neighbours[NeighBourDirection::S].size() > 0) {
      dir = 's';
      iid = &lvl.neighbours[NeighBourDirection::S][0];

      newPlayerPos.y = (-playerRect.h) + 1;
    }
  }
  else if (playerRect.bottom() <= 1) {
    if (lvl.neighbours[NeighBourDirection::N].size() > 0) {
      dir = 'n';
      iid = &lvl.neighbours[NeighBourDirection::N][0];

      int tilesTall = _ldtkProject->levels[*iid].tilesTall;

      newPlayerPos.y = (tilesTall * lvl.tileSize) - playerRect.h;
    }
  }

  if (iid != nullptr) {
    std::string id = *iid;
    printf("move dir(%c): %s\n", dir, id.c_str());
    v2i oldWorldPosition = _ldtkProject->levels[this->_level].worldPosition;
    v2i newWorldPosition = _ldtkProject->levels[id].worldPosition;
    // switchLevel(this->_level);

    v2i diff = newWorldPosition - oldWorldPosition;

    if (dir == 'e' || dir == 'w') {
      newPlayerPos.y -= diff.y;
    }
    else if (dir == 's' || dir == 'n') {
      newPlayerPos.x -= diff.x;
    }

    pendingLevel = { id, newPlayerPos };
  }

  _camera.update();
}

void GameplayScene::draw(Renderer* renderer) {
  RectF camera = _camera.getRect();
  // v2f cameraOffset = { (float)camera.x, (float)camera.y };
  Level* level = &_ldtkProject->levels[this->_level];

  // bg1->draw(renderer->getSdlRenderer(), 0);
  // bg2->draw(renderer->getSdlRenderer(), -camera.x * 0.04);

  for(auto layer : level->layers) {
    if (layer.def->type == LayerType::ENTITIES) {
      continue;
    }

    Tileset tileset = _ldtkProject->tilesetDefs[layer.def->tilesetId];

    for(uint16_t i = 0; i < layer.tiles.size(); i ++) {
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
    if (objRect.hasIntersection(&camera)) {
      obj->draw(renderer);
    }
  }

  hud->draw(renderer->getSdlRenderer());
  
  DebugPrinter::Instance()->draw(renderer);

  this->drawFade();
}
