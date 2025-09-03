#pragma once

#include "math.h"
#include "event.h"
#include <map>
#include <SDL2/SDL_events.h>

enum BUTTON {
  UP = 0x01,
  RIGHT = 0x02,
  DOWN = 0x03,
  LEFT = 0x04,
  JUMP = 0x05,
  ATTACK = 0x06,
  MENU = 0x07,
  DEBUG_FLY = 0x08,
  QUIT_BTN = 0x09,

  __LAST__ = 0xFF,
};

class InputHandler {
private:
  static InputHandler *_instance;
  std::map<int, BUTTON> _buttons;
  std::map<BUTTON, int> _buttons_translation;
  std::map<BUTTON, bool> _buttonsHeld;
  v2i _mousePos;

  InputHandler();

  ~InputHandler() {}

public:
  static InputHandler *Instance() {
    static InputHandler *_instance = nullptr;
    if (_instance == nullptr) {
      _instance = new InputHandler();
    }

    return _instance;
  }

  static void release() {
    delete _instance;
    _instance = NULL;
  }

  void addButton(int keyCode, BUTTON button);
  bool isHeld(BUTTON button);
  int buttonToSDLCode(BUTTON button);
  v2i getMousePosition();
  void onKeyPressed(BUTTON button);
  void onKeyReleased(BUTTON button);
  void pollEvent(SDL_Event event);
};
