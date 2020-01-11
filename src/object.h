#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "math.h"

class Object
{
protected:
  v2 position = { 0.0f, 0.0f };
  v2 size = { 32.0f, 32.0f };
  v2 velocity = { 0.0f, 0.0f };

  SDL_Rect textureRect;
  SDL_Rect hitBox;
  SDL_Rect prevHitBox;
  SDL_Texture* texture;

  std::string id = "";
  bool isSolid = false;
  float speed = 0.0f;
  std::vector<Object*> children;
  SDL_RendererFlip textureFlip = SDL_FLIP_NONE;
public:
  Object() {
    hitBox = { 0, 0, (int)size.x, (int)size.y };
  }

  std::string getId() { return id; }

  v2 getPosition() { return position; }
  void setPosition(v2 v) {
    position = v;
  }

  void setTexture(SDL_Texture* texture) { this->texture = texture; }

  SDL_Rect getHitBox() {
    return {
      (int)position.x + hitBox.x, (int)position.y + hitBox.y, hitBox.w, hitBox.h
    };
  }

  void setHitBox(SDL_Rect v) {
    hitBox = v;
  }

  void setTextureRect(SDL_Rect textureRect) { this->textureRect = textureRect; }

  SDL_Rect getRect() { return { 
    (int)position.x, (int)position.y, (int)size.x, (int)size.y };
  }

  v2 getSize() { return size; }
  void setSize(v2 v) { size = v; }

  bool getSolid() { return isSolid; }
  void setSolid(bool v) { isSolid = v; }

  bool intersectsWith(Object* other) {
    SDL_Rect r1 = { (int)position.x + hitBox.x, (int)position.y + hitBox.y, hitBox.w, hitBox.h };
    SDL_Rect r2 = other->getRect();
    return SDL_HasIntersection(&r1, &r2);
  }

  bool intersectsWithRect(SDL_Rect* other) {
    SDL_Rect r1 = { (int)position.x + hitBox.x, (int)position.y + hitBox.y, hitBox.w, hitBox.h };
    return SDL_HasIntersection(&r1, other);
  }

  virtual void update(float dt) {
    position.x += (velocity.x / 10) * dt;
    position.y += (velocity.y / 10) * dt;

    for (int i = 0; i < children.size(); i ++) {
      children[i]->update(dt);
    }
  }

  virtual void draw(SDL_Renderer* renderer, SDL_Rect origin) {
    prevHitBox = getHitBox();
    SDL_Rect r = { (int)position.x - origin.x, (int)position.y - origin.y, (int)textureRect.w, (int)textureRect.h };

    SDL_RenderCopyEx(renderer, texture, &textureRect, &r, 0, 0, textureFlip);

/*
    if (this->id == "player") {
      SDL_SetRenderDrawColor(renderer, 20, 255, 20, 100);
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_Rect currentHitbox = getHitBox();
      SDL_RenderFillRect(renderer, &currentHitbox);
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
*/

    for (int i = 0; i < children.size(); i ++) {
      children[i]->draw(renderer, r);
    }
  }
};
