#pragma once
#include <string>
#include "SDL.h"
#include "globals.h"
#include "font.h"
#include "math.h"
#include "timer.h"
#include "renderer.h"

#define DIALOGUE_BOX_HEIGHT 70

// We got the sprite font here:
// https://opengameart.org/content/bitmap-font

class Dialogue {
public:
  Dialogue(Renderer* renderer) {
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

    _frameTexture = SDL_CreateTextureFromSurface(_renderer->getSdlRenderer(), surface);

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

    _renderer->renderRectTexture(BOUNDS, _frameTexture, FRAME_WIDTH);

    int x = BOUNDS.x + PADDING;
    int y = BOUNDS.y + PADDING;

    const int GLYPH_WIDTH = _font->getGlyphWidth();
    const int GLYPH_HEIGHT = _font->getGlyphHeight();

    char prevChar = '0';
    for (int i = 0; _displayingMessage[i] != '\0'; i++) {
      if (i >= _currentCharacterIdx) {
        break;
      }

      if (_displayingMessage[i] == '\n') {
        y += GLYPH_HEIGHT;
        x = BOUNDS.x + PADDING;
        continue;
      }

      if (i < 11) {
        _font->drawLetter(_displayingMessage[i], { x, y }, FONT_COLOR::CHARACTER);
      }
      else {
        _font->drawLetter(_displayingMessage[i], { x, y });
      }

      // Check if next word causes an overflow
      // We only wanna do this in the beginning of a word
      int wordWidth = 0;
      if (_displayingMessage[i] == ' ') {
        for (int j = i + 1; _displayingMessage[j] != '\0' && _displayingMessage[j] != ' '; j++) {
          wordWidth += GLYPH_WIDTH;
        }
      }

      bool overflowing = x + wordWidth > BOUNDS.x + BOUNDS.w - (PADDING * 2);

      if (!overflowing) {
        x = x + GLYPH_WIDTH;
      }
      else {
        y += GLYPH_HEIGHT;
        x = BOUNDS.x + PADDING;
        
        // Remove next space if there's is one when moving to next line.
        if (_displayingMessage[i] != '\n' && _displayingMessage[i + 1] == ' ') {
          i ++;
        }
      }
    }
  }


private:
  const int LETTER_PAUSE = 65;
  const int FRAME_WIDTH = 16;
  const Rect BOUNDS = { 0, (WINDOW_HEIGHT / WINDOW_ZOOM) - DIALOGUE_BOX_HEIGHT, (WINDOW_WIDTH / WINDOW_ZOOM), DIALOGUE_BOX_HEIGHT };
  const int PADDING = 8;
  const char* FRAME_TEXTURE_PATH = "assets/dialogue.png";
  std::string _displayingMessage;
  SDL_Texture* _frameTexture;
  Renderer* _renderer;
  Timer _timer;
  int _currentCharacterIdx = 0;
  Font* _font;
};
