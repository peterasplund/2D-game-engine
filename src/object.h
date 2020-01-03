#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "math.h"

class Object
{
protected:
  v2 position = { 0.0f, 0.0f };
  v2 size = { 32.0f, 50.0f };
  v2 velocity = { 0.0f, 0.0f };

  SDL_Rect rect;
  SDL_Rect textureRect;

  SDL_Texture* texture;

  bool isSolid = false;
  float speed = 0.0f;
  std::vector<Object*> children;
public:
  Object() {
  }

  v2 getPosition() {
    return position;
  }

  void setPosition(v2 position) {
    this->position = position;
  }

  void setTexture(SDL_Texture* texture) {
    this->texture = texture;
  }

  void setTextureRect(SDL_Rect textureRect) {
    this->textureRect = textureRect;
  }

  v2 getSize() {
    return size;
  }

  void setSize(v2 size) {
    this->size = size;
  }

  void setSolid(bool v) {
    isSolid = v;
  }

  virtual void update(float dt) {
    position.x += (velocity.x / 10) * speed * dt;
    position.y += (velocity.y / 10) * speed * dt;

    for (int i = 0; i < children.size(); i ++) {
      children[i]->update(dt);
    }
  }

  virtual void draw(SDL_Renderer* renderer, SDL_Rect origin) {
    SDL_Rect r = { (int)position.x - origin.x, (int)position.y - origin.y, (int)size.x, (int)size.y };

    SDL_RenderCopy(renderer, texture, &textureRect, &r);

    for (int i = 0; i < children.size(); i ++) {
      children[i]->draw(renderer, r);
    }
  }
};
