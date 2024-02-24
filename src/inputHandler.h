#pragma once

#include <map>
#include "SDL.h"
#include "math.h"

enum BUTTON {
  UP,
  RIGHT,
  DOWN,
  LEFT,
  JUMP,
  ATTACK,
  MENU,
};

class InputHandler
{
private:
  static InputHandler* _instance;
  std::map<int, BUTTON> _buttons;
  std::map<BUTTON, bool> _buttonsHold;
  v2i _mousePos;

  InputHandler() {
    for (int i = UP; i != ATTACK; i++) {
      BUTTON button = static_cast<BUTTON>(i);

      _buttonsHold.insert(std::pair<BUTTON, bool>(button, false));
    }
  }

  ~InputHandler() {
  }

public:
  static InputHandler* Instance() {
    static InputHandler* _instance = nullptr;
    if (_instance == nullptr) {
      _instance = new InputHandler();
    }

    return _instance;
  }

  void addButton(int keyCode, BUTTON button) {
    _buttons[keyCode] = button;
  }

  bool isHeld(BUTTON button) {
    return _buttonsHold[button];
  }

  v2i getMousePosition() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return { x, y };
  }

  static void release() {
    delete _instance;
    _instance = NULL;
  }

  void pollEvent(SDL_Event event) {
    std::map<int, BUTTON>::iterator it;
    
    switch (event.type) {
      case SDL_KEYDOWN:
        for (it = _buttons.begin(); it != _buttons.end(); it ++) {
          if (event.key.keysym.sym == it->first) {
            _buttonsHold[it->second] = true;
          }
        }

        break;
      case SDL_KEYUP:
        for (it = _buttons.begin(); it != _buttons.end(); it ++) {
          if (event.key.keysym.sym == it->first) {
            _buttonsHold[it->second] = false;
          }
        }

        break;
    }
  }
};
