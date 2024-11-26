#pragma once

#include "SDL.h"
#include <string>

class Window
{
public:
  Window(const std::string &title, int width, int height, int zoom) :
  _title(title), _width(width), _height(height), _zoom(zoom) {
    _closed = !init(width, height, zoom);
  }

  ~Window();

  SDL_Renderer* getRenderer() {
    return _renderer;
  }

  void pollEvent(SDL_Event event);

  void clear() const;

  inline bool isClosed() { return _closed; }
private:
  bool init(int width, int height, int zoom);

  std::string _title;
  int _width;
  int _height;
  int _zoom;
  bool _closed = false;
  SDL_Window *_window = nullptr;
  SDL_Renderer *_renderer = nullptr;
};
