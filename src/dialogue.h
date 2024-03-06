#pragma once
#include <string>
#include "SDL.h"
#include "font.h"
#include "math.h"
#include "timer.h"

// https://opengameart.org/content/bitmap-font

class Dialogue {
public:
  Dialogue(SDL_Renderer* renderer) {
    _renderer = renderer;
    _font = new Font(_renderer);
    _timer = Timer();
  }

  bool init() {
    SDL_Surface* surface = IMG_Load(FRAME_TEXTURE_PATH);
    if (surface == NULL) {
      printf("Image load error: Path(%s) - Error(%s)\n", FRAME_TEXTURE_PATH, IMG_GetError());
      return false;
    }

    _frameTexture = SDL_CreateTextureFromSurface(_renderer, surface);

    if (_frameTexture == NULL) {
      printf("Failed to create texture\n");
    }

    if (surface == NULL || surface == NULL) {
      printf("Create texture error: %s\n", SDL_GetError());
      return false;
    }

    SDL_FreeSurface(surface); 

    return _font->init();
  }

  void message(std::string msg) {
    _displayingMessage = msg;
  }

  void update() {
    if (_displayingMessage != "") {
      if (_timer.elapsed() > LETTER_PAUSE) {
        // display next character
        if (_currentCharacterIdx < _displayingMessage.length()) {
          _currentCharacterIdx ++;
          _timer.reset();
        }
      }
    }
  }

  void draw() {
    if (_displayingMessage == "") {
      return;
    }

    drawFrame();
    _font->drawString(_displayingMessage.c_str(), {16,16}, _currentCharacterIdx);
  }


private:
  void drawCorner(const int x, const int y, const SDL_RendererFlip flip) {
    const int sr_x = flip & SDL_FLIP_HORIZONTAL ? 16 : 0;
    const int sr_y = flip & SDL_FLIP_VERTICAL & SDL_FLIP_VERTICAL ? 16 : 0;

    SDL_Rect sdl_sr = {
      sr_x,
      sr_y,
      16,
      16,
    };

    SDL_Rect sdl_dr = {
      x,
      y,
      16,
      16
    };

    SDL_RenderCopyEx(_renderer, _frameTexture, &sdl_sr, &sdl_dr, 0, 0, SDL_FLIP_NONE);
  }

  void drawFrame() {
    /*
    drawCorner(BOUNDS.x,      BOUNDS.y,      SDL_FLIP_NONE);
    drawCorner(BOUNDS.x + BOUNDS.w - 16, BOUNDS.y,      SDL_FLIP_HORIZONTAL);
    drawCorner(BOUNDS.x,      BOUNDS.y + BOUNDS.h - 16, SDL_FLIP_VERTICAL);
    drawCorner(BOUNDS.x + BOUNDS.w - 16, BOUNDS.y + BOUNDS.h - 16, (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));
    */
  }


  const int LETTER_PAUSE = 100;

  const Rect BOUNDS = { 0, 0, 200, 300 };
  const char* FRAME_TEXTURE_PATH = "assets/dialogue.png";
  std::string _displayingMessage;
  SDL_Texture* _frameTexture;
  SDL_Renderer* _renderer;
  Timer _timer;
  int _currentCharacterIdx = 0;
  Font* _font;
};
