#pragma once
#include "SDL.h"

class Timer
{
  private:
    Uint32 base;
  public:
    Timer() {
      base = SDL_GetTicks();
    }

    void reset() {
      base = SDL_GetTicks();
    }

    Uint32 elapsed() {
      return SDL_GetTicks() - base;
    }
};
