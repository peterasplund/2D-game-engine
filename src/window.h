#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "inputHandler.h"

class Window
{
public:
  Window(const std::string &title, int width, int height) :
  _title(title), _width(width), _height(height) {
    _closed = !init();
  }

  ~Window() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
  }

  SDL_Renderer* getRenderer() {
    return _renderer;
  }

  void pollEvents() {
    /*
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          _closed = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              _closed = true;
              break;
            case SDLK_a:
              _inputHandler->setState(BUTTON::LEFT, true);
              break;
            case SDLK_d:
              _inputHandler->setState(BUTTON::RIGHT, true);
              break;
          }
          break;
        case SDL_KEYUP:
          switch (event.key.keysym.sym) {
            case SDLK_a:
              _inputHandler->setState(BUTTON::LEFT, false);
              break;
            case SDLK_d:
              _inputHandler->setState(BUTTON::RIGHT, false);
              break;
          }
          break;
        default:
          break;
      }
    }
*/
    SDL_Event event;
    while( SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_QUIT:
            _closed = true;
            break;
          case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
              case SDLK_ESCAPE:
                _closed = true;
                break;
              case SDLK_a:
                _inputHandler->setState(BUTTON::LEFT, true);
                break;
              case SDLK_d:
                _inputHandler->setState(BUTTON::RIGHT, true);
                break;
              case SDLK_w:
                _inputHandler->setState(BUTTON::UP, true);
                break;
              case SDLK_s:
                _inputHandler->setState(BUTTON::DOWN, true);
                break;
            }
            break;
          case SDL_KEYUP:
            switch (event.key.keysym.sym) {
              case SDLK_a:
                _inputHandler->setState(BUTTON::LEFT, false);
                break;
              case SDLK_d:
                _inputHandler->setState(BUTTON::RIGHT, false);
                break;
              case SDLK_w:
                _inputHandler->setState(BUTTON::UP, false);
                break;
              case SDLK_s:
                _inputHandler->setState(BUTTON::DOWN, false);
                break;
            }
            break;
          default:
            break;
        }
      }
  }

  void clear() const {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
  }

  inline bool isClosed() { return _closed; }
private:
  InputHandler* _inputHandler;
  bool init() {
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

    if (_renderer == nullptr) {
      std::cerr << "Failed to create renderer.\n";
      return 0;
    }
  
    _inputHandler = InputHandler::Instance();

    return true;
  }

  std::string _title;
  int _width = 640;
  int _height = 480;
  bool _closed = false;
  SDL_Window *_window = nullptr;
  SDL_Renderer *_renderer = nullptr;
};
