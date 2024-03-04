#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "math.h"

/*
  average width +5
  ascender height +7
  descender height -2
  x-height +5
*/

class Font {
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

  void drawString(char* str, v2i position) {
    for (int i = 0; str[i] != '\0'; i++) {
      v2i glyph = charToGlyph(str[i]);
      drawGlyph(glyph, { 
          position.x + (i * GLYPH_WIDTH),
          position.y,
      });
    }
  }

private:
  SDL_Renderer* _renderer;
  SDL_Texture* _texture;

  const char* TEXTURE_PATH = "assets/font.png";
  const int GLYPH_WIDTH = 5;
  const int GLYPH_HEIGHT = 7;
  const int OFFSET_X = 3;
  const int OFFSET_Y = 40;

  v2i charToGlyph(char x) {
    // @TODO: implement
    return {0,0};
  }

  void drawGlyph(v2i glyph, v2i position) {
    SDL_Rect sdl_sr = {
      GLYPH_WIDTH * glyph.x,
      GLYPH_HEIGHT * glyph.y,
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
