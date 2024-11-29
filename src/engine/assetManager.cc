#include "assetManager.h"
#include "logger.h"
#include <SDL_image.h>

SDL_Texture *AssetManager::loadTexture(std::string path) {
  SDL_Texture *tex = NULL;

  SDL_Surface *surface = IMG_Load(path.c_str());
  if (surface == NULL) {
    LOG_FATAL("Image load error: Path(%s) - Error(%s)", path.c_str(),
              IMG_GetError());
    exit(1);
  }

  tex = SDL_CreateTextureFromSurface(_renderer, surface);

  if (tex == NULL) {
    LOG_FATAL("Failed to create texture");
    exit(1);
  }

  if (surface == NULL || surface == NULL) {
    LOG_FATAL("Create texture error: %s", SDL_GetError());
    exit(1);
  }

  SDL_FreeSurface(surface);

  return tex;
}

SDL_Texture *AssetManager::getTexture(std::string filename) {
  if (!this->initialized) {
    LOG_FATAL("ERROR: AssetManager is not initialized");
    exit(1);
  }

  std::string fullPath = SDL_GetBasePath();
  fullPath.append(filename);

  if (_textures[fullPath] == nullptr) {
    _textures[fullPath] = this->loadTexture(fullPath);
  }
  return _textures[fullPath];
}

bool AssetManager::freeTexture(std::string filename) {
  std::string fullPath = SDL_GetBasePath();
  fullPath.append(filename);

  if (_textures[fullPath] == nullptr) {
    SDL_DestroyTexture(_textures[fullPath]);
    _textures.erase(fullPath);
    return true;
  }

  return false;
}
