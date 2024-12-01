#pragma once

#include "../abstractGameobject.h"
#include "../bg.h"
#include "../components/camera.h"
#include "../dialogue.h"
#include "../engine/map.h"
#include "../entityManager.h"
#include "../hud.h"
#include "../map-hud.h"
#include "../obj/damageNumbers.h"
#include "../scene.h"

struct LevelTransition {
  int iid;
  v2f playerPosition;
};

class GameplayScene : public Scene {
private:
  Renderer *_renderer;
  camera _camera;
  AbstractGameObject *_player = nullptr;
  Bg *bg1;
  Bg *bg2;
  Hud *hud;
  MapHud *mapHud;
  Dialogue *dialogue;
  // Tilemap* tilemap;
  int _level;
  World *world;
  bool loaded = false;
  AbstractGameObject *instantiateGameObject(GAME_OBJECT);
  std::map<std::string, GAME_OBJECT> gameObjects;
  int transitionTimer = 0;
  bool isFadingIn = false;
  LevelTransition pendingLevel = {-1, {0, 0}};
  DamageNumbersSystem *damageNumberSystem;
  void drawFade();
  void instantiateEntitites(Level *level);

public:
  ~GameplayScene() {
    delete bg1;
    delete bg2;
    delete hud;
    delete mapHud;
  }
  GameplayScene(Renderer *renderer, World *world) : Scene(renderer) {
    _renderer = renderer;
    this->world = world;

    // find player in levels
    for (auto lvl : this->world->levels) {
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

  void init();
  void update(double dt);
  void draw(Renderer *renderer);

  void switchLevel(LevelTransition level);
};
