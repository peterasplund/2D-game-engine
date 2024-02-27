#pragma once

#include <memory>
#include "../abstractGameobject.h"
#include "../obj/player_overworld.h"
#include "../obj/door.h"
#include "../scene.h"
#include "../tilemap.h"
#include "../bg.h"
#include "../entityManager.h"
#include "../debugPrinter.h"
#include "../components/camera.h"


class OverworldScene : public Scene
{
private:
  SDL_Renderer* _renderer;
  camera _camera;
  Bg* bg1;
  Bg* bg2;
  Tilemap* tilemap;
  std::string _level;
  bool loaded = false;
  std::shared_ptr<AbstractGameObject> instantiateGameObject(GAME_OBJECT);
  std::map<std::string, GAME_OBJECT> gameObjects;
public:
  OverworldScene(SDL_Renderer* renderer, std::string level) : Scene(renderer) {
    _renderer = renderer;
    _level = level;
  }

  void init();
  void update(float dt);
  void draw(SDL_Renderer* renderer);
};
