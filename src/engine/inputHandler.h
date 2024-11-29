#pragma once

#include "math.h"
#include <map>
#include <SDL2/SDL_events.h>

enum BUTTON {
  UP,
  RIGHT,
  DOWN,
  LEFT,
  JUMP,
  ATTACK,
  MENU,
  __LAST__,
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
