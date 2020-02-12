#pragma once
#include "SDL.h"
#include <vector>
#include "scene.h"
#include "tilemap.h"
#include "bg.h"
#include "game/hud.h"
#include "game/gameState.h"
#include "../lib/entt/entt.hpp"

#include "components/position.h"
#include "components/velocity.h"
#include "components/renderable.h"
#include "components/camera.h"
#include "systems/renderableSystem.h"
#include "systems/velocitySystem.h"
#include "systems/animationSystem.h"
#include "systems/characterControllerSystem.h"
#include "systems/cameraSystem.h"
#include "systems/collisionSystem.h"
#include "systems/gravitySystem.h"
#include "systems/debugSystem.h"
#include "objects/player.h"
#include "objects/bat.h"
#include "objects/camera.h"

void foo(int a) {
  printf("CALLED FOO %d \n", a);
}

//typedef void(*CreateFunction)(int);
//typedef std::unordered_map<std::string, CreateFunction> PrefabMap;

typedef entt::entity(*CreateFunction)(entt::registry*, SDL_Renderer*, v2);
typedef std::unordered_map<std::string, CreateFunction> PrefabMap;

class DemoScene : public Scene
{
private:
  SDL_Renderer* _renderer;
  entt::registry registry;
  Tilemap* tilemap;
  //Hud* hud;
  //GameState* state;

  Bg* bg1;
  Bg* bg2;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
    _renderer = renderer;
  }

  void init() {
    //state = new GameState();
    //hud = new Hud(_renderer, state);
    //tilemap = new Tilemap("assets/maps/demo2.tmx", _renderer);
    tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);

    PrefabMap prefabs;
    prefabs.emplace("player", &createPlayer);
    prefabs.emplace("bat", &createBat);


    bg1 = new Bg("bgs/clouds.png", { 512.0f, 352.0f }, _renderer);
    bg2 = new Bg("bgs/town.png", { 512.0f, 352.0f }, _renderer);

    std::vector<std::pair<std::string, v2>> objects = tilemap->getObjects();
    for (std::pair<std::string, v2> pair : objects) {
      CreateFunction fn = prefabs.at(pair.first);
      v2 position = pair.second;

      if (fn != nullptr) {
        fn(&registry, _renderer, position);
      }
    }

    entt::entity cameraEntity = createCamera(&registry);
  }

  void update(float dt) {

    animationSystem(dt, registry);
    cameraSystem(registry);

    characterControllerSystem(InputHandler::Instance(), registry);
    gravitySystem(dt, registry);
    setCollisionSystemPrevCollisionBox(registry);
    velocitySystem(dt, registry);

    // Run collisions last
    collisionSystem(tilemap, registry);
  }

  void draw(SDL_Renderer* renderer) {
    auto view = registry.view<camera>();
    camera c;
    SDL_Rect cr;

    for (auto entity : view) {
      c = view.get<camera>(entity);
      cr = Camera::getRect(&c);
    }

    //bg1->draw(renderer, -cr.x * 0.04);
    //bg2->draw(renderer, -cr.x * 0.2);
    tilemap->draw(renderer, &c);
    renderableSystem(renderer, registry);
    //debugSystem(_renderer, registry);


    //hud->draw(renderer);

    SDL_RenderPresent(renderer);

  }
};
