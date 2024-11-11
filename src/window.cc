#include "window.h"
#include <iostream>

bool Window::init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
    return 0;
  }

  _window = SDL_CreateWindow(
    _title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    _width,
    _height,
    0
  );

  if (_window == nullptr) {
    std::cerr << "Failed to create window.\n";
    return 0;
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // For opacity
  SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

  ImguiLayer::Instance()->init(_window, _renderer);

  if (_renderer == nullptr) {
    std::cerr << "Failed to create renderer.\n";
    return 0;
  }

  SDL_RenderSetScale(_renderer, WINDOW_ZOOM, WINDOW_ZOOM);

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
