#pragma once
#include <core/math.h>
#include <core/renderer.h>
#include <core/map.h>
#include <core/settings.h>

#include "globals.h"
#include "obj/player.h"
#include "components/camera.h"

const int FADE_TIME = 150;

struct LevelTransition {
  int iid;
  v2f playerPosition;
};

class LevelManager {
public:
  LevelTransition pendingLevel = {-1, {0, 0}};
  Timer transitionTimer;
  int _level;
  World* _world;
  bool isFadingToBlack = true;

  bool isTransitioning() {
    return pendingLevel.iid != -1 && isFadingToBlack;
  }

  LevelManager(World* world) {
    _world = world;
    this->_level = -1;

    // find player in levels
    for (auto lvl : _world->levels) {
      for (auto layer : lvl.layers) {
        for (auto entity : layer.entities) {
          if (entity.identifier == "Player") {
            _level = lvl.iid;
          }
        }
      }
    }

    if (_level == -1) {
      LOG_FATAL("Couldn't find start position for player");
      exit(1);
    }
  }

  void setLevel(int l) {
    this->_level = l;
    camera* _camera = &EntityManager::Instance()->_camera;
    Level *lvl = &_world->levels[this->_level];
    std::vector<AbstractGameObject *> entities =
        EntityManager::Instance()->_entities;

    for (AbstractGameObject *entity : EntityManager::Instance()->_entities) {
      if (!entity->_persist) {
        entity->dead = true;
      }
    }

    EntityManager::Instance()->setTileMap(lvl);
    EntityManager::Instance()->instantiateLevelEntitites(_world, lvl);

    _camera->setBounds({lvl->tilesWide * lvl->tileSize, lvl->tilesTall * lvl->tileSize});
  }

  void switchLevel(LevelTransition level) {
    setLevel(level.iid);

    obj::Player* player = (obj::Player*) EntityManager::Instance()->_player;
    player->setPosition(pendingLevel.playerPosition);
    player->_collidable.update(player->_position);

    (&EntityManager::Instance()->_camera)->update();

    // Update every object once to prevent flashing bug. This happens
    // since we pause objects and don't update them during a transition.
    for (const auto &obj : EntityManager::Instance()->getEntities()) {
      if (obj != nullptr) {
        obj->update(0.0f);
      }
    }
    EntityManager::Instance()->update();
  }

  void update(double dt) {
    if (pendingLevel.iid != -1) {
      if (isFadingToBlack) {
        if (transitionTimer.elapsed() > FADE_TIME) {
          transitionTimer.reset();
          isFadingToBlack = false;
          switchLevel(pendingLevel);
          return;
        }
      }
      else {
        if (transitionTimer.elapsed() > FADE_TIME) {
          transitionTimer.reset();
          isFadingToBlack = true;
          pendingLevel = {-1, {0, 0}};
          return;
        }
      }

      if (isFadingToBlack) {
        return;
      }
    }

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
        newPlayerPos.y = -playerRect.h;
      }
    } else if (playerRect.bottom() <= 1) {
      if (lvl.neighbours[NeighBourDirection::N].size() > 0) {
        dir = 'n';

        nextLevel = _world->getLevelByCell({playerCellPos.x, playerCellPos.y - 1});
        nextLevel->cell.debugInt();

        int tilesTall = _world->levels[nextLevel->iid].tilesTall;

        newPlayerPos.y =
            (tilesTall * lvl.tileSize) - playerRect.h; //- (playerRect.h / 2);
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
      transitionTimer.reset();
    }
  }

  void drawFade(Renderer* renderer) {
    if (pendingLevel.iid != -1) {
      float timerValue = (float)transitionTimer.elapsed() / (float)FADE_TIME;

      float fadeFraction = isFadingToBlack 
        ? lerp(0, 1, easing(timerValue))
        : lerp(1, 0, easing(timerValue));

      int fade = fadeFraction * 255;

      int windowWidth = gameSettings().windowWidth;
      int windowHeight = gameSettings().windowHeight;
      int zoom = gameSettings().zoom;
      Rect fadeRect = {0, 0, windowWidth * zoom, windowHeight * zoom};
      renderer->setColor(0, 0, 0, fade);
      renderer->renderRectFilled(&fadeRect, false);
    }
  }
};
