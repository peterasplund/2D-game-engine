#pragma once
#include <SDL2/SDL.h>
#include "window.h"
#include "scene.h"
#include <map>

class SceneManager
{
private:
  std::map<std::string, Scene*> _scenes;
  std::string _current;
public:
  SceneManager() {
  }

  ~SceneManager() {
      _scenes.clear();
  }

  void init() {
    _scenes[_current]->init();
  }

  void update(float dt) {
    _scenes[_current]->update(dt);
  }

  void draw(SDL_Renderer* renderer) {
    _scenes[_current]->draw(renderer);
  }

  void addScene(std::string name, Scene *scene) {
    _scenes[name] = scene;
  }

  bool removeScene(std::string name) {
    if (_scenes[name] != nullptr) {
      delete _scenes[name];
      return true;
    }
    return false;
  }

  bool gotoScene(std::string name) {
    if (_scenes[name] != nullptr) {
      _current = name;
      ((Scene*)_scenes[name])->init();
      return true;
    }
    return false;
  }
};
