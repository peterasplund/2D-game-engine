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
#include "../map.h"

class GameplayScene : public Scene
{
private:
  Renderer* _renderer;
  camera _camera;
  std::shared_ptr<AbstractGameObject> _player;
  Bg* bg1;
  Bg* bg2;
  Hud* hud;
  //Tilemap* tilemap;
  std::string _level;
  World* _ldtkProject;
  bool loaded = false;
  std::shared_ptr<AbstractGameObject> instantiateGameObject(GAME_OBJECT);
  std::map<std::string, GAME_OBJECT> gameObjects;
public:
  GameplayScene(Renderer* renderer, World* ldtkProject, std::string level) : Scene(renderer) {
    _renderer = renderer;
    _level = level;
    _ldtkProject = ldtkProject;
  }

  void init();
  void update(float dt);
  void draw(Renderer* renderer);

  void switchLevel(std::string level);
};
