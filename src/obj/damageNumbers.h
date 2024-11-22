#pragma once

#include "../assetManager.h"
#include "../renderer.h"
#include "../inputHandler.h"
#include <cstdlib>

#define NUMBER_SPEED 0.2f
#define NUMBER_LIFETIME 600.0f

struct DamageNumber {
  v2f position;
  int damage;
  int timer = 0;
  bool alive = false;
  char dmgString[5];
  int dmgStringLen;

  void init() {
    sprintf(dmgString, "%d", damage);
    dmgStringLen = strlen(dmgString);
  }

  void draw(Renderer* renderer, SDL_Texture* texture) {
    //printf("y: %f, yPos: %d\n", value, yPos);

    for (int i = 0; i < dmgStringLen; i++) {

      int value = dmgString[i] - 48;
      Rect textureRect = {
        value * 8,
        13,
        8,
        10,
      };

      Rect dr = { (int)round(position.x) + (i * 5), (int)round(position.y), textureRect.w, textureRect.h };

      int alpha = ((timer / NUMBER_LIFETIME) * -1) * 255;

      SDL_SetTextureAlphaMod(texture, alpha);
      renderer->renderTexture(texture, &textureRect, &dr, SDL_FLIP_NONE);
    }
  }

  void update(float dt) {
    timer += dt;

    float value = (timer / NUMBER_LIFETIME) * (M_PI / 2); // 0 - PI/2
    position.y -= (cos(value) * NUMBER_SPEED);

    if (timer >= NUMBER_LIFETIME) {
      alive = false;
    }
  }
};

class DamageNumbersSystem {
  public:
    void init() {

      numbersCount = 0;
      int tw = 8;
      int th = 12;

      texture = AssetManager::Instance()->getTexture("assets/font.png");
    }

    void addNumber(int damage, v2i position) {
      numbers[numbersCount].position = position;
      numbers[numbersCount].timer = 0;
      numbers[numbersCount].damage = damage;
      numbers[numbersCount].alive = true;
      numbers[numbersCount].init();

      numbersCount ++;
    }

    void update(float dt) {
      for (int i = 0; i < numbersCount; i++) {
        if (numbers[i].alive) {
          numbers[i].update(dt);
        }


        if (!numbers[i].alive) {
          numbers[i] = numbers[numbersCount - 1];
          numbersCount --;

          if (i > 0) {
            i --;
          }
        }
      }

      if (InputHandler::Instance()->Instance()->isHeld(BUTTON::ATTACK)) {
        if (numbers[numbersCount - 1].timer == 0 || numbers[numbersCount - 1].timer > 140) {
          int value = ((float)(rand()) / (float)RAND_MAX) * 255;
          addNumber(value, { 64, 64});
        }
      }
    }

    void draw(Renderer* renderer) {
      for (int i = 0; i < numbersCount; i++) {
        if (numbers[i].alive) {
          numbers[i].draw(renderer, texture);
        }
      }
    }

  protected:
    Rect textureRect = { 0, 0, 8, 12 };
    v2i position;
    SDL_Texture* texture;
    float elapsedTime = 0.0f;
    Renderer* renderer;
    DamageNumber numbers[1024];
    int numbersCount;
};
