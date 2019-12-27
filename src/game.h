#pragma once
#include <SDL2/SDL.h>
#include "window.h"
#include "sceneManager.h"
#include "scene.h"
#include "demoScene.h"

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
const int WINDOW_FPS    = 60;

class Game
{
public:
  Game() {
    SceneManager* _sceneManager = new SceneManager();
    Window window("Hello world", WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Renderer* renderer = window.getRenderer();

    DemoScene* _demoScene = new DemoScene(renderer);
    _sceneManager->addScene("demo", _demoScene);
    _sceneManager->gotoScene("demo");

    while (!window.isClosed()) {
      LAST = NOW;
      NOW = SDL_GetPerformanceCounter();
      deltaTime = (double)((NOW - LAST)*1000 / SDL_GetPerformanceFrequency() );

      window.clear();
      window.pollEvents();
      _sceneManager->update(deltaTime);
      _sceneManager->draw(renderer);
    }
  }

  ~Game() {

  }

private:
  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;

};
