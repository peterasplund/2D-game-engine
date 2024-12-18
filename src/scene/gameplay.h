#pragma once

#include "../abstractGameobject.h"
#include "../bg.h"
#include "../dialogue.h"
#include "../engine/map.h"
#include "../entityManager.h"
#include "../hud.h"
#include "../map-hud.h"
#include "../obj/damageNumbers.h"
#include "../scene.h"
#include "../levelManager.h"

class GameplayScene : public Scene {
private:
  Renderer *_renderer;
  AbstractGameObject *_player = nullptr;
  Bg *bg1;
  Bg *bg2;
  Hud *hud;
  MapHud *mapHud;
  Dialogue *dialogue;
  World *world;
  bool loaded = false;
  AbstractGameObject *instantiateGameObject(GAME_OBJECT);
  DamageNumbersSystem *damageNumberSystem;
  void drawFade();
  void onSwitchLevel();
  LevelManager* levelManager;

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
  }

  void init();
  void update(double dt);
  void draw(Renderer *renderer);
};
