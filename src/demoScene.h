#pragma once
#include "stdafx.h"
#include <vector>
#include "scene.h"
#include "tilemap.h"
#include "bg.h"
#include "game/hud.h"
#include "game/gameState.h"

#include "components/position.h"
#include "components/velocity.h"
#include "components/renderable.h"
#include "components/camera.h"
#include "components/batSpawner.h"
#include "systems/renderableSystem.h"
#include "systems/velocitySystem.h"
#include "systems/animationSystem.h"
#include "systems/characterControllerSystem.h"
#include "systems/cameraSystem.h"
#include "systems/collisionSystem.h"
#include "systems/gravitySystem.h"
#include "systems/debugSystem.h"
#include "systems/lifetimeSystem.h"
#include "systems/batSpawnerSystem.h"
#include "systems/skeletonAISystem.h"
#include "systems/batSystem.h"
#include "objects/player.h"
#include "objects/bat.h"
#include "objects/camera.h"
#include "objects/movingPlatform.h"
#include "objects/skeleton.h"
#include "objects/batSpawner.h"

typedef entt::entity(*CreateFunction)(entt::registry*, SDL_Renderer*, TiledObject);
typedef std::unordered_map<std::string, CreateFunction> PrefabMap;

class DemoScene : public Scene
{
private:
  SDL_Renderer* _renderer;
  entt::registry registry;
  entt::dispatcher dispatcher{};
  //Hud* hud;
  //GameState* state;

  Bg* bg1;
  Bg* bg2;
public:
  DemoScene(SDL_Renderer* renderer) : Scene(renderer) {
    _renderer = renderer;
  }

  void init() {
    // bind events
    destroyOnTouchSolidBind(&dispatcher);
    hurtOnTouchHurtableBind(&dispatcher);

    //state = new GameState();
    //hud = new Hud(_renderer, state);
    Tilemap* tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);
    //tilemap = new Tilemap("assets/maps/demo3.tmx", _renderer);

    tilemap->addTilesToRegistry(&registry);

    PrefabMap prefabs = {
      { "player", &createPlayer },
      //{ "bat", &createBat },
      { "batSpawner", &createBatSpawner },
      { "skeleton", &createSkeleton }
    };

    bg1 = new Bg("bgs/clouds.png", { 512.0f, 352.0f }, _renderer);
    bg2 = new Bg("bgs/town.png", { 512.0f, 352.0f }, _renderer);

    registry.create();

    std::vector<TiledObject> objects = tilemap->getObjects();
    for (TiledObject o : objects) {
      if (prefabs.find(o.name) == prefabs.end()) {
        continue;
      }

      CreateFunction fn = prefabs.at(o.name);

      if (fn != nullptr) {
        fn(&registry, _renderer, o);
      }
    }

    createMovingPlatform(&registry, _renderer, { 64, 192 }, { .5f, 0 });

    createCamera(&registry, tilemap);

    // Lag bats
    // for (int i = 0; i < 2000; i++) { createBat(&registry, _renderer, { 0, 0 }); }

    // @TODO: set prevRect to rect in collidable constructor instead.
    initCollisionSystem(&registry);
  }

  void update(float dt) {
    animationSystem(dt, &registry);
    cameraSystem(&registry);

    characterControllerSystem(InputHandler::Instance(), &registry, _renderer);
    gravitySystem(dt, &registry);
    setCollisionSystemPrevCollisionBox(&registry);
    velocitySystem(dt, &registry);
    lifetimeSystem(dt, &registry);
    batSystem(&registry);

    batSpawnerSystem(_renderer, &registry);
    skeletonAISystem(&registry);

    // Run collisions last
    collisionSystem(&registry, &dispatcher);
  }

  void draw(SDL_Renderer* renderer) {
    auto view = registry.view<camera>();
    camera c;
    SDL_Rect cr;

    for (auto entity : view) {
      c = view.get<camera>(entity);
      cr = Camera::getRect(&c);
    }

    bg1->draw(renderer, -cr.x * 0.04);
    bg2->draw(renderer, -cr.x * 0.2);
    renderableSystem(renderer, &registry);
    //debugSystem(_renderer, &registry);

    //hud->draw(renderer);

    SDL_RenderPresent(renderer);

  }
};
