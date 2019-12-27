#pragma once
#include <SDL2/SDL.h>
#include <map>

enum BUTTON {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

class InputHandler
{
private:
  static InputHandler* _instance;
  std::map<BUTTON, bool> _state;

  InputHandler() {
    for (int i = UP; i != LEFT; i++) {
      BUTTON button = static_cast<BUTTON>(i);

      _state.insert(std::pair<BUTTON, bool>(button, false));
    }
  }

  ~InputHandler() {
  }

public:
  static InputHandler* Instance() {
  if (_instance == NULL) {
      _instance = new InputHandler();
    }

    return _instance;
  }

  void setState(BUTTON button, bool state) {
    if (_state[button] != state) {
      _state[button] = state;
    }
  }

  bool isPressed(BUTTON button) {
    return _state[button];
  }

  static void release() {
    delete _instance;
    _instance = NULL;
  }
};

InputHandler* InputHandler::_instance = 0;
