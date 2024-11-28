#include "window.h"
#include "logger.h"
#include <iostream>

bool Window::init(const char *title, int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    LOG_FATAL("Failed to initialize SDL: %s", SDL_GetError());
    return 0;
  }

  _width = width;
  _height = height;

  _window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, _width, _height, 0);

  if (_window == nullptr) {
    LOG_FATAL("Failed to create window.");
    exit(1);
  }

  _renderer = SDL_CreateRenderer(
      _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // For opacity
  SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

  if (_renderer == nullptr) {
    LOG_FATAL("Failed to create renderer.");
    exit(1);
  }

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
