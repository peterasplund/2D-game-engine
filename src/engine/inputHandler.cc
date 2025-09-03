#include "inputHandler.h"

InputHandler::InputHandler() {
  for (int i = BUTTON::UP; i != BUTTON::__LAST__; i++) {
    BUTTON button = static_cast<BUTTON>(i);

    _buttonsHeld.insert(std::pair<BUTTON, bool>(button, false));
  }
}

void InputHandler::onKeyPressed(BUTTON button) {
  event_context ctx;
  ctx.data.u16[0] = button;
  event_fire(EVENT_CODE_KEY_PRESSED, 0, ctx);
}

void InputHandler::onKeyReleased(BUTTON button) {
  event_context ctx;
  ctx.data.u16[0] = button;
  event_fire(EVENT_CODE_KEY_RELEASED, 0, ctx);
}

void InputHandler::pollEvent(SDL_Event event) {
  std::map<int, BUTTON>::iterator it;

  switch (event.type) {
  case SDL_KEYDOWN:
    for (it = _buttons.begin(); it != _buttons.end(); it++) {
      if (event.key.keysym.sym == it->first) {
        if (!_buttonsHeld[it->second]) {
          onKeyPressed(it->second);
        }
        _buttonsHeld[it->second] = true;
      }
    }

    break;
  case SDL_KEYUP:
    for (it = _buttons.begin(); it != _buttons.end(); it++) {
      if (event.key.keysym.sym == it->first) {
        if (_buttonsHeld[it->second]) {
          onKeyReleased(it->second);
        }

        _buttonsHeld[it->second] = false;
      }
    }

    break;
  }
}

void InputHandler::addButton(int keyCode, BUTTON button) {
  _buttons[keyCode] = button;
  _buttons_translation[button] = keyCode;
}

bool InputHandler::isHeld(BUTTON button) { return _buttonsHeld[button]; }

int InputHandler::buttonToSDLCode(BUTTON button) {
  return _buttons_translation[button];
}

v2i InputHandler::getMousePosition() {
  int x, y;
  SDL_GetMouseState(&x, &y);
  return {x, y};
}
