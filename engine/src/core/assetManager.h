#pragma once

#include <SDL2/SDL_render.h>
#include <map>
#include <string>

// @TODO: use reference counting to destroy when no one uses the texture or
// wahtererver
class AssetManager {
private:
  std::map<std::string, SDL_Texture *> _textures;
  SDL_Renderer *_renderer = nullptr;
  bool initialized = false;

  AssetManager() {}
  AssetManager(const AssetManager&) = delete;
  AssetManager& operator=(const AssetManager&) = delete;
  AssetManager(AssetManager&&) = delete;
  AssetManager& operator=(AssetManager&&) = delete;

  ~AssetManager() {
    release();
  }

  void releaseTextures() {
    for (auto tex : _textures) {
      if (tex.second != nullptr) {
        SDL_DestroyTexture(tex.second);
      }
    }

    _textures.clear();
    initialized = false;
    _renderer = nullptr;
  }

  bool freeTexture(std::string filename);
  SDL_Texture *loadTexture(std::string path);

public:
  static AssetManager *Instance() {
    static AssetManager instance;
    return &instance;
  }

  void init(SDL_Renderer *renderer) {
    this->_renderer = renderer;
    this->initialized = true;
  }

  static void release() { Instance()->releaseTextures(); }

  SDL_Texture *getTexture(std::string filename);
};
