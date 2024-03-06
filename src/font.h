#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "math.h"

// https://opengameart.org/content/bitmap-font

class Font {
public:
  Font(SDL_Renderer* renderer) {
    _renderer = renderer;
  }

  bool init() {
    SDL_Surface* surface = IMG_Load(TEXTURE_PATH);
    if (surface == NULL) {
      printf("Image load error: Path(%s) - Error(%s)\n", TEXTURE_PATH, IMG_GetError());
      return false;
    }

    _texture = SDL_CreateTextureFromSurface(_renderer, surface);

    if (_texture == NULL) {
      printf("Failed to create texture\n");
    }

    if (surface == NULL || surface == NULL) {
      printf("Create texture error: %s\n", SDL_GetError());
      return false;
    }

    SDL_FreeSurface(surface); 

    return true;
  }

  // @TODO: move overflowing logic into dialogue.h
  void drawString(const char* str, v2i position, int stopAt) {
    v2i glyph;

    int x = position.x;
    int y = position.y;

    char prevChar = '0';
    for (int i = 0; str[i] != '\0'; i++) {
      if (i >= stopAt) {
        break;
      }

      glyph = charToGlyph(str[i]);
      drawGlyph(glyph, { x, y });

      // Check if next word causes an overflow
      // We only wanna do this in the beginning of a word
      int wordWidth = 0;
      if (str[i] == ' ') {
        for (int j = i + 1; str[j] != '\0' && str[j] != ' '; j++) {
          wordWidth += GLYPH_WIDTH;
        }
      }

      bool overflowing = x + wordWidth > BOUNDS.w;

      if (!overflowing) {
        x = x + GLYPH_WIDTH;
      }
      else {
        printf("Overflowing\n");
        y += GLYPH_HEIGHT;
        x = position.x;
        
        // Remove next space if there's is one when moving to next line.
        if (str[i] != '\n' && str[i + 1] == ' ') {
          i ++;
        }
      }
    }
  }

private:
  SDL_Renderer* _renderer;
  SDL_Texture* _texture;

  const char* TEXTURE_PATH = "assets/font.png";
  const int GLYPH_WIDTH = 8;
  const int GLYPH_HEIGHT = 12;
  const Rect BOUNDS = { 0, 0, 200, 300 };

  v2i charToGlyph(char x) {
    int asciiPos = x - 32;
    int top = asciiPos / 16 * GLYPH_HEIGHT;
    int left = asciiPos % 16 * GLYPH_WIDTH;

    return {left, top};
  }

  void drawGlyph(v2i glyph, v2i position) {
    SDL_Rect sdl_sr = {
      glyph.x,
      glyph.y,
      GLYPH_WIDTH,
      GLYPH_HEIGHT,
    };

    SDL_Rect sdl_dr = {
      position.x,
      position.y,
      GLYPH_WIDTH,
      GLYPH_HEIGHT
    };

    SDL_RenderCopyEx(_renderer, _texture, &sdl_sr, &sdl_dr, 0, 0, SDL_FLIP_NONE);
  }

};
