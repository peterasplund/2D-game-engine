#pragma once

#include <SDL2/SDL.h>

class Window {
public:
  Window(const char *title, int width, int height)
      : _title(title), _width(width), _height(height) {
    _closed = !init(_title, _width, _height);
  }

  ~Window();

  SDL_Renderer *getRenderer() { return _renderer; }

  SDL_Window *getWindow() { return _window; }

  void pollEvent(SDL_Event event);

  void clear() const;

  inline bool isClosed() { return _closed; }

private:
  bool init(const char *title, int width, int height);

  const char *_title;
  int _width;
  int _height;
  bool _closed = false;
  SDL_Window *_window = nullptr;
  SDL_Renderer *_renderer = nullptr;
};
