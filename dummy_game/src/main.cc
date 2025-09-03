#include <stdio.h>
#include <core/application.h>
#include <core/inputHandler.h>
#include <core/ldtk.h>

#include "scene/gameplay.h"
#include "sceneManager.h"
#include "imgui_layer.h"

int main() {
  Application* app = Application::Instance();
  app_config config = {
      .window_x = 0,
      .window_y = 0,
      .window_width = 480,
      .window_height = 272,
      .zoom = 4,
      .window_name = "Demo game",
      .vsync = true,
      .max_frame_rate = 60,
  };

  app->initialize(&config);
  ImguiLayer::Instance()->init(app->window->getWindow(), app->window->getRenderer());

  InputHandler::Instance()->addButton(SDLK_w, BUTTON::UP);
  InputHandler::Instance()->addButton(SDLK_s, BUTTON::DOWN);
  InputHandler::Instance()->addButton(SDLK_a, BUTTON::LEFT);
  InputHandler::Instance()->addButton(SDLK_d, BUTTON::RIGHT);
  InputHandler::Instance()->addButton(SDLK_k, BUTTON::JUMP);
  InputHandler::Instance()->addButton(SDLK_j, BUTTON::ATTACK);
  InputHandler::Instance()->addButton(SDLK_p, BUTTON::MENU);
  InputHandler::Instance()->addButton(SDLK_SPACE, BUTTON::DEBUG_FLY);
  InputHandler::Instance()->addButton(SDLK_u, BUTTON::QUIT_BTN);

  World world = createWorld("assets/maps/LDtk_test.ldtk");

  SceneManager *_sceneManager = new SceneManager(app->renderer);

  GameplayScene *_gameplayScene = new GameplayScene(app->renderer, &world);
  _sceneManager->addScene("gameplay", _gameplayScene);

  _sceneManager->gotoScene("gameplay", Transition::NONE);

  app->run();

  ImguiLayer *imgui = ImguiLayer::Instance();

	while (!app->window->isClosed()) {
    app->beforeUpdate();

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			for (const auto &obj : EntityManager::Instance()->getEntities()) {
				obj->handleEvent(&event);
			}

			if (event.key.keysym.sym == SDLK_LSHIFT &&
					event.key.state == SDL_PRESSED) {
				  imgui->toggleVisible();
			}

			app->window->pollEvent(event);
			InputHandler::Instance()->pollEvent(event);
			imgui->processEvents(&event);
		}

    _sceneManager->update(app->getDeltaTime());
    _sceneManager->draw(app->renderer);

    imgui->drawBegin();
    imgui->debugEntities(EntityManager::Instance()->getEntities());
    // EntityManager::Instance()->imgui();
    imgui->drawEnd();

    app->afterUpdate();
	}

  return 0;
}
