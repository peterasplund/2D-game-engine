#pragma once

#include "globals.h"
#include "SDL.h"
#include "imgui_layer.h"

class Window
{
public:
  Window(const std::string &title, int width, int height) :
  _title(title), _width(width), _height(height) {
    _closed = !init();
  }

  ~Window();

  SDL_Renderer* getRenderer() {
    return _renderer;
  }

  void pollEvent(SDL_Event event);

  void clear() const;

  inline bool isClosed() { return _closed; }
private:
  bool init();

  std::string _title;
  int _width;
  int _height;
  bool _closed = false;
  SDL_Window *_window = nullptr;
  SDL_Renderer *_renderer = nullptr;
};
