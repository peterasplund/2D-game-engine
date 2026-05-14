#include "application.h"
#include "assetManager.h"
#include "event.h"
#include "inputHandler.h"

Application *Application::Instance() {
  static Application instance;
  return &instance;
}

bool Application::initialize(app_config *config) {
  if (this->is_initialized) {
    // Error here
    return false;
  }

  this->is_initialized = true;
  this->window =
      new Window(config->window_name, config->window_width * config->zoom,
                 config->window_height * config->zoom);

  SDL_Renderer *sdl_renderer = window->getRenderer();
  SDL_RenderSetScale(sdl_renderer, config->zoom, config->zoom);
  this->renderer = new Renderer(sdl_renderer);

  // Initialize subsystems
  event_initialize();
  AssetManager::Instance()->init(sdl_renderer);

  return true;
}

Application::Application() {
  is_initialized = false;
  is_running = false;
  is_suspended = false;
  last_time = 0;
  window = nullptr;
  renderer = nullptr;
  now = 0;
  last = 0;
  end = 0;
  delta_time = 0;
}

Application::~Application() {
  event_shutdown();

  delete this->window;
  delete this->renderer;
}

void Application::beforeUpdate() {
  last = now;
  now = SDL_GetPerformanceCounter();

  delta_time = (double)((now - last) * 1000.0f / SDL_GetPerformanceFrequency());

  renderer->clearScreen();
}

void Application::afterUpdate() {
  renderer->present();
  end = SDL_GetPerformanceCounter();

  if (!config.vsync) {
    float elapsedMS =
        (end - now) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(1000 / (float)config.max_frame_rate - elapsedMS);
  }
}

void Application::run() { now = SDL_GetPerformanceCounter(); }
