#pragma once

#include <memory>
#include "../abstractGameobject.h"
#include "../obj/player.h"
#include "../obj/door.h"
#include "../scene.h"
#include "../bg.h"
#include "../entityManager.h"
#include "../debugPrinter.h"
#include "../components/camera.h"
#include "../hud.h"
#include "../LDtkParser.h"

class GameplayScene : public Scene
{
private:
  Renderer* _renderer;
  camera _camera;
  Bg* bg1;
  Bg* bg2;
  Hud* hud;
  Tilemap* tilemap;
  std::string _level;
  Project* _ldtkProject;
  bool loaded = false;
  std::shared_ptr<AbstractGameObject> instantiateGameObject(GAME_OBJECT);
  std::map<std::string, GAME_OBJECT> gameObjects;
public:
  GameplayScene(Renderer* renderer, Project* ldtkProject, std::string level) : Scene(renderer) {
    _renderer = renderer;
    _level = level;
    _ldtkProject = ldtkProject;
  }

  void init();
  void update(float dt);
  void draw(Renderer* renderer);
};
