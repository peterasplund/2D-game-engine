#pragma once
#include "SDL.h"
#include "window.h"
#include "inputHandler.h"
#include "sceneManager.h"
#include "scene.h"
#include "demoScene.h"

const int WINDOW_WIDTH  = 512 * 2;
const int WINDOW_HEIGHT = 352 * 2;
const int WINDOW_FPS    = 60;

class Game
{
public: Game() {
    InputHandler::Instance()->addButton(SDLK_w, BUTTON::UP);
    InputHandler::Instance()->addButton(SDLK_s, BUTTON::DOWN);
    InputHandler::Instance()->addButton(SDLK_a, BUTTON::LEFT);
    InputHandler::Instance()->addButton(SDLK_d, BUTTON::RIGHT);
    InputHandler::Instance()->addButton(SDLK_k, BUTTON::JUMP);
    InputHandler::Instance()->addButton(SDLK_j, BUTTON::ATTACK);

    SceneManager* _sceneManager = new SceneManager();
    Window window("Hello world", WINDOW_WIDTH, WINDOW_HEIGHT);
    Timer fpsTimer;
    SDL_Renderer* renderer = window.getRenderer();

    DemoScene* _demoScene = new DemoScene(renderer);
    _sceneManager->addScene("demo", _demoScene);
    _sceneManager->gotoScene("demo");

    while (!window.isClosed()) {
      LAST = NOW;
      NOW = SDL_GetPerformanceCounter();
      deltaTime = (double)((NOW - LAST)*1000 / SDL_GetPerformanceFrequency() );

      window.clear();

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        window.pollEvent(event);
        InputHandler::Instance()->pollEvent(event);
      }

      _sceneManager->update(deltaTime);
      _sceneManager->draw(renderer);

      if( (fpsTimer.elapsed() < 1000 / WINDOW_FPS)) {
        //Sleep the remaining frame time
        // comment out for now since we're using VSYNC
        // SDL_Delay( ( 1000 / WINDOW_FPS ) - fpsTimer.elapsed() );
      }

    }
  }

  ~Game() {

  }

private:
  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;

};
