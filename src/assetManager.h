#pragma once

#include <string>
#include <map>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"

// @TODO: use reference counting to destroy when no one uses the texture or wahtererver
class AssetManager
{
private:
  std::map<std::string, SDL_Texture*> _textures;
  SDL_Renderer* _renderer;

  AssetManager() {
  }

  ~AssetManager() {
    for (auto tex : _textures) {
      if (tex.second != NULL) {
        SDL_DestroyTexture(tex.second);
      }
    }

    _textures.clear();
  }

  bool freeTexture(std::string filename) {
    std::string fullPath = SDL_GetBasePath();
    fullPath.append(filename);

    if (_textures[fullPath] == nullptr) {
      SDL_DestroyTexture(_textures[fullPath]);
      _textures.erase(fullPath);
      return true;
    }
    
    return false;
  }

  SDL_Texture* loadTexture(std::string path) {
    SDL_Texture* tex = NULL;

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL) {
      printf("Image load error: Path(%s) - Error(%s)\n", path.c_str(), IMG_GetError());
      return NULL;
    }

    tex = SDL_CreateTextureFromSurface(_renderer, surface);

    if (tex == NULL) {
      printf("Failed to create texture\n");
    }

    if (surface == NULL || surface == NULL) {
      printf("Create texture error: %s\n", SDL_GetError());
      return NULL;
    }

    SDL_FreeSurface(surface); 

    return tex;
  }

public:
  static AssetManager* Instance() {
    static AssetManager* _instance = nullptr;
    if (_instance == nullptr) {
      _instance = new AssetManager();
    }
    else {
      if (_instance->_renderer == nullptr) {
        printf("ERROR: AssetManager is not initialized\n");
      }
    }

    return _instance;
  }

  void init(SDL_Renderer* renderer) {
    this->_renderer = renderer;
  }

  static void release() {
  }

  SDL_Texture* getTexture(std::string filename) {
    std::string fullPath = SDL_GetBasePath();
    fullPath.append(filename);

    if (_textures[fullPath] == nullptr) {
      _textures[fullPath] = this->loadTexture(fullPath);
    }
    return _textures[fullPath];
  }
};
