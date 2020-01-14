#pragma once
#include "SDL.h"
#include <map>

class AssetManager
{
private:
  static AssetManager* _instance;
  std::map<std::string, SDL_Texture*> _textures;
  SDL_Renderer* _renderer;

  AssetManager(SDL_Renderer* renderer) {
    this->_renderer = renderer;
  }

  ~AssetManager() {
    for (auto tex : _textures) {
      if (tex.second != NULL) {
        SDL_DestroyTexture(tex.second);
      }
    }

    _textures.clear();
  }

  SDL_Texture* loadTexture(std::string path) {
    SDL_Texture* tex = NULL;

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL) {
      printf("Image load error: Path(%s) - Error(%s)\n", path.c_str(), IMG_GetError());
      return NULL;
    }

    tex = SDL_CreateTextureFromSurface(_renderer, surface);

    if (surface == NULL || surface == NULL) {
      printf("Create texture error: %s\n", SDL_GetError());
      return NULL;
    }

    SDL_FreeSurface(surface); 

    return tex;
  }

public:
  static AssetManager* Instance(SDL_Renderer* renderer) {
    if (_instance == 0) {
      _instance = new AssetManager(renderer);
    }

    return _instance;
  }
  static AssetManager* Instance() {
    if (_instance != 0) {
      return _instance;
    }

    return NULL;
  }

  static void release() {
    delete _instance;
    _instance = NULL;
  }

  SDL_Texture* getTexture(std::string filename) {
    std::string fullPath = SDL_GetBasePath();
    fullPath.append("../assets/" + filename);

    if (_textures[fullPath] == nullptr) {
      _textures[fullPath] = this->loadTexture(fullPath);
    }
    return _textures[fullPath];
  }
};

AssetManager* AssetManager::_instance = 0;
