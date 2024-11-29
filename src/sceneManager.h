#pragma once

#include "engine/math.h"
#include "engine/renderer.h"
#include "globals.h"
#include "scene.h"
#include <map>

const int FADE_SPEED = 8;

enum class Transition {
  FADE,
  NONE,
};

class SceneManager {
private:
  std::map<std::string, Scene *> _scenes;
  std::string _current;
  Renderer *_renderer;
  Rect _fadeRect;
  int _fade = 0;
  std::string _transitioningTo = "";
  bool _fadingOut = true;

  void switchScene() {
    if (!_transitioningTo.empty()) {
      _current = _transitioningTo;
      ((Scene *)_scenes[_current])->init();
    }
  }

public:
  SceneManager(Renderer *renderer) {
    _renderer = renderer;
    _fadeRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
  }

  ~SceneManager() { _scenes.clear(); }

  void init() { _scenes[_current]->init(); }

  void update(float dt) {
    _scenes[_current]->update(dt);

    if (!_transitioningTo.empty()) {
      if (_fadingOut) {
        _fade += FADE_SPEED;
        if (_fade >= 255) {
          _fadingOut = false;
          switchScene();
        }
      } else {
        _fade -= FADE_SPEED;
        if (_fade <= 0) {
          _transitioningTo = "";
          _fadingOut = true;
        }
      }
    }
  }

  void draw(Renderer *renderer) {
    _scenes[_current]->draw(renderer);
    drawFade();
  }

  void addScene(std::string name, Scene *scene) { _scenes[name] = scene; }

  bool removeScene(std::string name) {
    if (_scenes[name] != nullptr) {
      delete _scenes[name];
      return true;
    }
    return false;
  }

  void drawFade() {
    int fade = max(min(_fade, 255), 0);
    if (!_transitioningTo.empty()) {
      _renderer->setColor(0, 0, 0, fade);
      _renderer->renderRectFilled(&_fadeRect, false);
    }
  }

  bool isUpdating() { return !_transitioningTo.empty(); }

  std::string currentScene() { return _current; }

  bool gotoScene(std::string name, Transition transition) {
    if (_scenes[name] == nullptr) {
      return false;
    }

    // Just go to scene directly if it's the first one
    if (_current.empty()) {
      _current = name;
      ((Scene *)_scenes[_current])->init();
      return true;
    }

    switch (transition) {
    case Transition::NONE:
      _current = name;
      ((Scene *)_scenes[_current])->init();
      break;
    case Transition::FADE:
      _transitioningTo = name;
      break;
    }

    return true;
  }
};
