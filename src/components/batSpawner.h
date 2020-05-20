#pragma once
#include "../stdafx.h"
#include "../timer.h"
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