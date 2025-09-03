#pragma once

#include <core/math.h>
#include <core/renderer.h>
#include <core/timer.h>
#include "font.h"
#include <string>
#include <vector>

#define DIALOGUE_BOX_HEIGHT 70

enum class TagType {
  COLOR,
};

struct MsgTag {
  TagType type;
  int start;
  int length;
};

struct Message {
  std::vector<MsgTag> tags;
  std::string msg;
};

// We got the sprite font here:
// https://opengameart.org/content/bitmap-font

class Dialogue {
public:
  void destroy() { delete _font; }

  static Dialogue *Instance() {
    static Dialogue *_instance = nullptr;
    if (_instance == nullptr) {
      _instance = new Dialogue();
    }

    return _instance;
  }

  bool isDisplayingMessage() { return _displayingMessage.length() > 0; }

  bool init(Renderer *renderer);

  Message parseMessage(std::string s);

  void message(std::string msg) {
    _currentCharacterIdx = 0;
    _displayingMessage = msg;
  }

  void update();
  void draw();

private:
  const uint32_t LETTER_PAUSE = 65;
  const int FRAME_WIDTH = 16;
  // const Rect BOUNDS = { 0, (WINDOW_HEIGHT / WINDOW_ZOOM) -
  // DIALOGUE_BOX_HEIGHT, (WINDOW_WIDTH / WINDOW_ZOOM), DIALOGUE_BOX_HEIGHT };
  const Rect BOUNDS = {0, 0, 300, 100};
  const int PADDING = 8;
  const char *FRAME_TEXTURE_PATH = "assets/dialogue.png";
  std::string _displayingMessage;
  SDL_Texture *_frameTexture;
  Renderer *_renderer;
  Timer _timer;
  uint32_t _currentCharacterIdx = 0;
  Font *_font;
};
