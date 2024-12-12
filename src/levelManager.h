#pragma once
#include "engine/math.h"
#include "engine/renderer.h"
#include "engine/map.h"
#include "globals.h"
#include "obj/player.h"
#include "components/camera.h"

const float LEVEL_FADE_SPEED = 2.3f;

struct LevelTransition {
  int iid;
  v2f playerPosition;
};

class LevelManager {
public:
  LevelTransition pendingLevel = {-1, {0, 0}};
  int transitionTimer = 0;
  int _level;
  World* _world;
  bool isFadingIn = false;
  camera* _camera;

  bool isTransitioning() {
    return isFadingIn || pendingLevel.iid != -1;
  }

  LevelManager(World* world, camera* _camera) {
    _world = world;
    this->_camera = _camera;

    // find player in levels
    for (auto lvl : this->_world->levels) {
      for (auto layer : lvl.layers) {
        for (auto entity : layer.entities) {
          if (entity.identifier == "Player") {
            this->_level = lvl.iid;
          }
        }
      }
    }

    if (_level == -1) {
      LOG_FATAL("Couldn't find start position for player");
      exit(1);
    }
  }

  void switchLevel(LevelTransition level) {
    this->_level = level.iid;
    Level *lvl = &_world->levels[this->_level];
    std::list<AbstractGameObject *> entities =
        EntityManager::Instance()->_entities;

    for (AbstractGameObject *entity : EntityManager::Instance()->_entities) {
      if (!entity->_persist) {
        entity->dead = true;
      }
    }

    EntityManager::Instance()->setTileMap(lvl);

    /*
    instantiateEntitites(lvl);

    _player->setPosition(pendingLevel.playerPosition);
    _player->_collidable.update(_player->_position);

    _camera.setBounds(
        {lvl->tilesWide * lvl->tileSize, lvl->tilesTall * lvl->tileSize});
    _camera.pos.x = 0.0f;
    */
  }

  void update(double dt) {
    if (pendingLevel.iid != -1) {
      if (!isFadingIn) {
        transitionTimer += (LEVEL_FADE_SPEED * dt);
        if (transitionTimer >= 255) {
          isFadingIn = true;
          switchLevel(pendingLevel);
        }
      } else {
        transitionTimer -= (LEVEL_FADE_SPEED * dt);
        if (transitionTimer <= 0) {
          isFadingIn = false;
          pendingLevel = {-1, {0, 0}};
        }
      }

      if (!isFadingIn) {
        return;
      }
    }
  }

  void update2() {
    obj::Player* player = (obj::Player*) EntityManager::Instance()->_player;
    char dir = '-';
    Level *nextLevel = nullptr;
    v2f newPlayerPos = player->getPosition();
    v2i playerCellPos = _world->getCellByPx(player->_position, this->_level);
    RectF playerRect = player->_collidable.addBoundingBox(player->_position);
    Level lvl = _world->levels[_level];

    if (playerRect.left() + 1 >= lvl.tilesWide * lvl.tileSize) {
      if (lvl.neighbours[NeighBourDirection::E].size() > 0) {
        dir = 'e';
        nextLevel = _world->getLevelByCell({playerCellPos.x + 1, playerCellPos.y});
        newPlayerPos.x = -playerRect.w - (playerRect.w / 2);
      }
    } else if (playerRect.right() <= 1) {
      if (lvl.neighbours[NeighBourDirection::W].size() > 0) {
        dir = 'w';
        nextLevel = _world->getLevelByCell({playerCellPos.x - 1, playerCellPos.y});

        int tilesWide = _world->levels[nextLevel->iid].tilesWide;

        newPlayerPos.x =
            (tilesWide * lvl.tileSize) - playerRect.w - (playerRect.w / 2);
      }
    } else if (playerRect.top() + 1 >= lvl.tilesTall * lvl.tileSize) {
      if (lvl.neighbours[NeighBourDirection::S].size() > 0) {
        dir = 's';

        nextLevel = _world->getLevelByCell({playerCellPos.x, playerCellPos.y + 1});
        newPlayerPos.y = -playerRect.h - (playerRect.h / 2);
      }
    } else if (playerRect.bottom() <= 1) {
      if (lvl.neighbours[NeighBourDirection::N].size() > 0) {
        dir = 'n';

        nextLevel = _world->getLevelByCell({playerCellPos.x, playerCellPos.y - 1});
        nextLevel->cell.debugInt();

        int tilesTall = _world->levels[nextLevel->iid].tilesTall;

        newPlayerPos.y =
            (tilesTall * lvl.tileSize) - playerRect.h - (playerRect.h / 2);
      }
    }

    if (nextLevel != nullptr) {
      int id = nextLevel->iid;
      LOG_INFO("goto level: %d", nextLevel->iid);
      v2i oldWorldPosition = _world->levels[this->_level].cellPositionPx;
      v2i newWorldPosition = _world->levels[id].cellPositionPx;

      v2i diff = newWorldPosition - oldWorldPosition;

      if (dir == 'e' || dir == 'w') {
        newPlayerPos.y -= diff.y;
      } else if (dir == 's' || dir == 'n') {
        newPlayerPos.x -= diff.x;
      }

      pendingLevel = {id, newPlayerPos};
    }
  }

  void drawFade(Renderer* renderer) {
    if (pendingLevel.iid != -1) {
      int fade = max(min(transitionTimer, 255), 0);
      Rect fadeRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
      renderer->setColor(0, 0, 0, fade);
      renderer->renderRectFilled(&fadeRect, false);
    }
  }
};
