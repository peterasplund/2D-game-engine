#pragma once
#include "SDL.h"

int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}

const int FADE_SPEED = 8;

typedef (*void) transition_callback;

enum class TransitionType {
  FADE,
  NONE,
};

class Transition {
  int _fade = 0;
  SDL_Rect _fadeRect;
  bool _fadingOut = true;
  void (*onComplete);


  public:
  Transition(void (*onComplete)(void)) {

  }

  void update(float dt) {
    if (_fadingOut) {
      _fade += FADE_SPEED;
      if (_fade >= 255) {
        _fadingOut = false;
        switchScene();
      }
    }
    else {
      _fade -= FADE_SPEED;
      if (_fade <= 0) {
        _transitioningTo = "";
        _fadingOut = true;
      }
    }
  }

  void drawTransition(SDL_Renderer* _renderer) {
    int fade = max(min(_fade, 255), 0);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, fade);
    SDL_RenderFillRect(_renderer, &_fadeRect);

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, fade);
  }
};
