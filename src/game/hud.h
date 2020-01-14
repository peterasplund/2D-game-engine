#pragma once
#include "SDL.h"
#include "gameState.h"

class Hud
{
private:
  GameState* _state;
  SDL_Rect _background;
public:
  Hud(SDL_Renderer* renderer, GameState* state) {
    _state = state;
    int bgHeight = 128;
    _background = { 0, 480 - bgHeight, 640, bgHeight };
  }

  void draw(SDL_Renderer* renderer) {
    // printf("hp: %d\n", _state->life);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &_background);
  }
};
