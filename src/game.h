#pragma once

#include "engine/inputHandler.h"
#include "engine/ldtk.h"
#include "engine/renderer.h"
#include "engine/timer.h"
#include "engine/window.h"
#include "globals.h"
#include "scene/gameplay.h"
#include "imgui_layer.h"
#include "sceneManager.h"

class Game {
public:
  Game() {
    InputHandler::Instance()->addButton(SDLK_w, BUTTON::UP);
    InputHandler::Instance()->addButton(SDLK_s, BUTTON::DOWN);
    InputHandler::Instance()->addButton(SDLK_a, BUTTON::LEFT);
    InputHandler::Instance()->addButton(SDLK_d, BUTTON::RIGHT);
    InputHandler::Instance()->addButton(SDLK_k, BUTTON::JUMP);
    InputHandler::Instance()->addButton(SDLK_j, BUTTON::ATTACK);
    InputHandler::Instance()->addButton(SDLK_p, BUTTON::MENU);

    Window window("Hello world", WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_Renderer *sdl_renderer = window.getRenderer();
    SDL_RenderSetScale(sdl_renderer, WINDOW_ZOOM,
                       WINDOW_ZOOM); // Set 4x zoom for the pixelated look

    ImguiLayer::Instance()->init(window.getWindow(), window.getRenderer());
    AssetManager::Instance()->init(sdl_renderer);

    Renderer *renderer = new Renderer(sdl_renderer);

    World world = createWorld("assets/maps/LDtk_test.ldtk");

    SceneManager *_sceneManager = new SceneManager(renderer);
    Timer fpsTimer;

    GameplayScene *_gameplayScene = new GameplayScene(renderer, &world);
    _sceneManager->addScene("gameplay", _gameplayScene);

    _sceneManager->gotoScene("gameplay", Transition::NONE);

    while (!window.isClosed()) {
      LAST = NOW;
      NOW = SDL_GetPerformanceCounter();

      deltaTime = std::min<double>(
          (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency()),
          MAX_DELTA_TIME);

      renderer->clearScreen();

      SDL_Event event;
      ImguiLayer *imgui = ImguiLayer::Instance();

      while (SDL_PollEvent(&event)) {
        for (const auto &obj : EntityManager::Instance()->getEntities()) {
          obj->handleEvent(&event);
        }

        if (event.key.keysym.sym == SDLK_LSHIFT &&
            event.key.state == SDL_PRESSED) {
          imgui->toggleVisible();
        }

        window.pollEvent(event);
        InputHandler::Instance()->pollEvent(event);
        imgui->processEvents(&event);
      }

      _sceneManager->update(deltaTime);
      _sceneManager->draw(renderer);

      imgui->drawBegin();
      imgui->debugEntities(EntityManager::Instance()->getEntities());
      // EntityManager::Instance()->imgui();
      imgui->drawEnd();

      renderer->present();

      // if( (fpsTimer.elapsed() < 1000 / WINDOW_FPS)) {
      // Sleep the remaining frame time
      //  comment out for now since we're using VSYNC
      // SDL_Delay( ( 1000 / WINDOW_FPS ) - fpsTimer.elapsed() );
      //}
    }

    delete _gameplayScene;
    delete _sceneManager;
    EntityManager::release();
  }

  ~Game() {}

private:
  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;
};
