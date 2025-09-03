#pragma once

#include <core/renderer.h>
#include <core/math.h>

// https://opengameart.org/content/bitmap-font

enum class FONT_COLOR {
  NONE,
  CHARACTER,
  KEY_ITEM,
};

class Font {
public:
  Font(Renderer *renderer) { _renderer = renderer; }

  bool init() {
    Sprite *sprite = new Sprite();
    ;
    _renderer->loadSprite(TEXTURE_PATH, sprite);
    _sprite = sprite;

    return true;
  }

  void drawLetter(const char letter, v2i position,
                  FONT_COLOR color = FONT_COLOR::NONE) {
    setTextColor(color);
    v2i glyph = charToGlyph(letter);
    drawGlyph(glyph, position);
  }

  void drawString(const char* str, v2i position,
                  FONT_COLOR color = FONT_COLOR::NONE) {
    int len = strlen(str);
    for (int i = 0; i < len; i ++) {
      drawLetter(str[i], { position.x + (i * GLYPH_WIDTH), position.y }, color);
    }
  }

  int getGlyphWidth() { return GLYPH_WIDTH; }

  int getGlyphHeight() { return GLYPH_HEIGHT; }

private:
  Renderer *_renderer;
  Sprite *_sprite;

  const char *TEXTURE_PATH = "assets/font.png";
  const int GLYPH_WIDTH = 8;
  const int GLYPH_HEIGHT = 12;
  const Rect BOUNDS = {0, 0, 200, 300};

  v2i charToGlyph(char x) {
    int asciiPos = x - 32;
    int top = asciiPos / 16 * GLYPH_HEIGHT;
    int left = asciiPos % 16 * GLYPH_WIDTH;

    return {left, top};
  }

  void setTextColor(FONT_COLOR color) {
    switch (color) {
    case FONT_COLOR::KEY_ITEM:
      SDL_SetTextureColorMod(_sprite->texture, 200, 15, 15);
      break;
    case FONT_COLOR::CHARACTER:
      SDL_SetTextureColorMod(_sprite->texture, 15, 200, 15);
      break;
    default:
      SDL_SetTextureColorMod(_sprite->texture, 255, 255, 255);
      break;
    }
  }

  void drawGlyph(v2i glyph, v2i position) {
    SDL_Texture *texture = _sprite->texture;
    SDL_Rect sdl_sr = {
        glyph.x,
        glyph.y,
        GLYPH_WIDTH,
        GLYPH_HEIGHT,
    };

    SDL_Rect sdl_dr = {position.x, position.y, GLYPH_WIDTH, GLYPH_HEIGHT};

    // SDL_SetTextureColorMod(texture, 255, 0, 0);

    SDL_RenderCopyEx(_renderer->getSdlRenderer(), texture, &sdl_sr, &sdl_dr, 0,
                     0, SDL_FLIP_NONE);
  }
};
