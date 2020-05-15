#pragma once
#include "SDL.h"
#include "SDL_timer.h"
#include "../objects/bat.h"

struct batSpawner {
    enum Direction { rightToLeft, leftToRight };

   batSpawner() {
       this->timer = Timer(); 
   }

    int frequency = 1000;
    Direction direction = Direction::rightToLeft;
    SDL_Rect bounds;
    Timer timer;
};