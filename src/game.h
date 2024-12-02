#pragma once

#include "engine/inputHandler.h"
#include "engine/ldtk.h"
#include "engine/renderer.h"
#include "engine/window.h"
#include "engine/settings.h"
#include "font.h"
#include "globals.h"
#include "imgui_layer.h"
#include "scene/gameplay.h"
#include "sceneManager.h"

class Game {
public:
  Game() {
    gameSettings().vsync = true;
    gameSettings().maxFrameRate = 60; // this doesn't matter when vsync is enabled

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
    debugFont = new Font(renderer);
    debugFont->init();

    World world = createWorld("assets/maps/LDtk_test.ldtk");

    SceneManager *_sceneManager = new SceneManager(renderer);

    GameplayScene *_gameplayScene = new GameplayScene(renderer, &world);
    _sceneManager->addScene("gameplay", _gameplayScene);

    _sceneManager->gotoScene("gameplay", Transition::NONE);

    NOW = SDL_GetPerformanceCounter();
    while (!window.isClosed()) {
      LAST = NOW;
      NOW = SDL_GetPerformanceCounter();

      deltaTime = (double)((NOW - LAST) * 1000.0f / SDL_GetPerformanceFrequency());

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

      int fps = floor(1000 / deltaTime);
      _sceneManager->update(deltaTime);
      _sceneManager->draw(renderer);
      sprintf(fpsString, "fps: %d", fps);
      debugFont->drawString(fpsString, {32, 32});

      imgui->drawBegin();
      imgui->debugEntities(EntityManager::Instance()->getEntities());
      // EntityManager::Instance()->imgui();
      imgui->drawEnd();

      renderer->present();

      END = SDL_GetPerformanceCounter();

      if (!gameSettings().vsync) {
        float elapsedMS = (END - NOW) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(1000 / (float)gameSettings().maxFrameRate -  elapsedMS);
      }
    }

    delete _gameplayScene;
    delete _sceneManager;
    EntityManager::release();
  }

  ~Game() {}

private:
  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  Uint64 END = 0;
  Font *debugFont;
  double deltaTime = 0;
  char fpsString[10];
};
