#include "window.h"

#include <iostream>
#include "logger.h"

bool Window::init(int width, int height, int zoom) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    LOG_FATAL("Failed to initialize SDL: %s.", SDL_GetError());
    return 0;
  }

  _width = width;
  _height = height;
  _zoom = zoom;

  _window = SDL_CreateWindow(
    _title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    _width,
    _height,
    0
  );

  if (_window == nullptr) {
    LOG_FATAL("Failed to create window.");
    return 0;
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // For opacity
  SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

  // ImguiLayer::Instance()->init(_window, _renderer);

  if (_renderer == nullptr) {
    LOG_FATAL("Failed to create renderer.");
    return 0;
  }

  SDL_RenderSetScale(_renderer, zoom, zoom);

  return true;
}

Window::~Window() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

void Window::pollEvent(SDL_Event event) {
  switch (event.type) {
    case SDL_QUIT:
      _closed = true;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          _closed = true;
          break;
      }
      break;
  }
}
