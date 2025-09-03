#pragma once

#include <core/assetManager.h>
#include <core/math.h>
#include "hudMeter.h"

class Hud {
protected:
  HudMeter *hpMeter;
  HudMeter *mpMeter;

public:
  Hud() {
    hpMeter = new HudMeter({ 4, 4 , 154, 9 }, { 106, 190, 48, 255 } );
    mpMeter = new HudMeter({ 4, 16 , 154, 9 }, { 48, 70, 190, 255 } );
  }

  ~Hud() {
    delete hpMeter;
    delete mpMeter;
  }

  virtual void draw(SDL_Renderer *renderer, int hp, int maxHp) {
    hpMeter->render(renderer, hp, maxHp);
    mpMeter->render(renderer, 100, 100);
  }
};
