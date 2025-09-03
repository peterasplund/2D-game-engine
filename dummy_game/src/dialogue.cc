#include "dialogue.h"
#include <core/logger.h>
#include <SDL2/SDL_image.h>

bool Dialogue::init(Renderer *renderer) {
  _renderer = renderer;
  _font = new Font(_renderer);
  _timer = Timer();

  SDL_Surface *surface = IMG_Load(FRAME_TEXTURE_PATH);
  if (surface == NULL) {
    LOG_ERROR("Image load error: Path(%s) - Error(%s)", FRAME_TEXTURE_PATH,
              IMG_GetError());
    return false;
  }

  _frameTexture =
      SDL_CreateTextureFromSurface(_renderer->getSdlRenderer(), surface);

  if (_frameTexture == NULL) {
    LOG_ERROR("Failed to create texture");
  }

  if (surface == NULL || surface == NULL) {
    LOG_ERROR("Create texture error: %s", SDL_GetError());
    return false;
  }

  SDL_FreeSurface(surface);

  return _font->init();
}

Message Dialogue::parseMessage(std::string s) {
  std::vector<MsgTag> tags;
  std::string msg = "";
  msg.resize(s.length());

  int msgIdx = 0;
  bool inTag = false;
  for (uint32_t i = 0; i < s.length(); i++) {
    msg[msgIdx] = s[i];

    if (s[i] == '<') {
      if (!inTag) {
        inTag = true;
      } else {
        inTag = false;
      }
    }

    msgIdx++;
  }

  return {
      tags,
      msg,
  };
}

void Dialogue::update() {
  if (_displayingMessage != "") {
    if (_timer.elapsed() > LETTER_PAUSE) {
      // display next character
      if (_currentCharacterIdx < _displayingMessage.length()) {
        _currentCharacterIdx++;
        _timer.reset();
      } else {
        // @TODO: check input too
        _displayingMessage = "";
      }
    }
  }
}

void Dialogue::draw() {
  if (_displayingMessage == "") {
    return;
  }

  _renderer->renderRectTexture(BOUNDS, _frameTexture, FRAME_WIDTH);

  int x = BOUNDS.x + PADDING;
  int y = BOUNDS.y + PADDING;

  const int GLYPH_WIDTH = _font->getGlyphWidth();
  const int GLYPH_HEIGHT = _font->getGlyphHeight();

  for (uint32_t i = 0; _displayingMessage[i] != '\0'; i++) {
    if (i >= _currentCharacterIdx) {
      break;
    }

    if (_displayingMessage[i] == '\n') {
      y += GLYPH_HEIGHT;
      x = BOUNDS.x + PADDING;
      continue;
    }

    if (i < 11) {
      _font->drawLetter(_displayingMessage[i], {x, y}, FONT_COLOR::CHARACTER);
    } else {
      _font->drawLetter(_displayingMessage[i], {x, y});
    }

    // Check if next word causes an overflow
    // We only wanna do this in the beginning of a word
    int wordWidth = 0;
    if (_displayingMessage[i] == ' ') {
      for (int j = i + 1;
           _displayingMessage[j] != '\0' && _displayingMessage[j] != ' '; j++) {
        wordWidth += GLYPH_WIDTH;
      }
    }

    bool overflowing = x + wordWidth > BOUNDS.x + BOUNDS.w - (PADDING * 2);

    if (!overflowing) {
      x = x + GLYPH_WIDTH;
    } else {
      y += GLYPH_HEIGHT;
      x = BOUNDS.x + PADDING;

      // Remove next space if there's is one when moving to next line.
      if (_displayingMessage[i] != '\n' && _displayingMessage[i + 1] == ' ') {
        i++;
      }
    }
  }
}
