#include "inputHandler.h"
#include "abstractGameobject.h"

InputHandler::InputHandler() {
  for (int i = BUTTON::UP; i != BUTTON::__LAST__; i++) {
    BUTTON button = static_cast<BUTTON>(i);

    _buttonsHeld.insert(std::pair<BUTTON, bool>(button, false));
  }
}

void InputHandler::onKeyPressed(BUTTON button) {
  for(const auto &obj : EntityManager::Instance()->getEntities()) {
    obj->onInputPressed(button);
  }
}

void InputHandler::onKeyReleased(BUTTON button) {
  for(const auto &obj : EntityManager::Instance()->getEntities()) {
    obj->onInputReleased(button);
  }
}

void InputHandler::pollEvent(SDL_Event event) {
  std::map<int, BUTTON>::iterator it;
  
  switch (event.type) {
    case SDL_KEYDOWN:
      for (it = _buttons.begin(); it != _buttons.end(); it ++) {
        if (event.key.keysym.sym == it->first) {
          if (!_buttonsHeld[it->second]) {
            onKeyPressed(it->second);
          }
          _buttonsHeld[it->second] = true;
        }
      }

      break;
    case SDL_KEYUP:
      for (it = _buttons.begin(); it != _buttons.end(); it ++) {
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

bool InputHandler::isHeld(BUTTON button) {
  return _buttonsHeld[button];
}

int InputHandler::buttonToSDLCode(BUTTON button) {
  return _buttons_translation[button];
}

v2i InputHandler::getMousePosition() {
  int x, y;
  SDL_GetMouseState(&x, &y);
  return { x, y };
}
