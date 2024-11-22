#pragma once

#include <memory>
#include <vector>
#include "../abstractGameobject.h"
#include "../obj/player.h"
#include "../obj/door.h"
#include "../scene.h"
#include "../bg.h"
#include "../entityManager.h"
#include "../debugPrinter.h"
#include "../components/camera.h"
#include "../hud.h"
#include "../map.h"
#include "../map-hud.h"
#include "../obj/damageNumbers.h"
#include "../dialogue.h"

const int LEVEL_FADE_SPEED = 14;

struct LevelTransition {
  int iid;
  v2f playerPosition;
};

class GameplayScene : public Scene
{
private:
  Renderer* _renderer;
  camera _camera;
  AbstractGameObject* _player = nullptr;
  Bg* bg1;
  Bg* bg2;
  Hud* hud;
  MapHud* mapHud;
  Dialogue* dialogue;
  //Tilemap* tilemap;
  int _level;
  World* world;
  bool loaded = false;
  AbstractGameObject* instantiateGameObject(GAME_OBJECT);
  std::map<std::string, GAME_OBJECT> gameObjects;
  int transitionTimer = 0;
  bool isFadingIn = false;
  LevelTransition pendingLevel = { -1, {0,0} };
  DamageNumbersSystem* damageNumberSystem;
  void drawFade();
  void instantiateEntitites(Level* level);
public:
  ~GameplayScene() {
    delete bg1;
    delete bg2;
    delete hud;
    delete mapHud;
  }
  GameplayScene(Renderer* renderer, World* world) : Scene(renderer) {
    _renderer = renderer;
    this->world = world;

    // find player in levels
    for(auto lvl : this->world->levels) {
      for(auto layer : lvl.layers) {
        for(auto entity : layer.entities) {
          if (entity.identifier == "Player") {
            this->_level = lvl.iid;
          }
        }
      }
    }

    if (_level == -1) {
      printf("Couldn't find start position for player\n");
      exit(1);
    }
  }

  void init();
  void update(float dt);
  void draw(Renderer* renderer);

  void switchLevel(LevelTransition level);
};
