#pragma once

#include <SDL2/SDL_timer.h>

class Timer
{
  private:
    Uint32 base;
  public:
    Timer() {
      base = SDL_GetTicks();
    }
    ~Timer() {}

    void reset() {
      base = SDL_GetTicks();
    }

    Uint32 elapsed() {
      return SDL_GetTicks() - base;
    }
};
