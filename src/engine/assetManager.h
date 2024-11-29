#pragma once

#include <SDL2/SDL_render.h>
#include <map>
#include <string>

// @TODO: use reference counting to destroy when no one uses the texture or
// wahtererver
class AssetManager {
private:
  std::map<std::string, SDL_Texture *> _textures;
  SDL_Renderer *_renderer;
  bool initialized = false;

  AssetManager() {}

  ~AssetManager() {
    for (auto tex : _textures) {
      if (tex.second != NULL) {
        SDL_DestroyTexture(tex.second);
      }
    }

    _textures.clear();
  }

  bool freeTexture(std::string filename);
  SDL_Texture *loadTexture(std::string path);

public:
  static AssetManager *Instance() {
    static AssetManager *_instance = nullptr;
    if (_instance == nullptr) {
      _instance = new AssetManager();
    }

    return _instance;
  }

  void init(SDL_Renderer *renderer) {
    this->_renderer = renderer;
    this->initialized = true;
  }

  static void release() {}

  SDL_Texture *getTexture(std::string filename);
};
