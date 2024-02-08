#pragma once

#include "stdafx.h"
#include "components/renderable.h"
#include "components/velocity.h"
#include "components/gravity.h"
#include "components/collidable.h"
#include "math.h"

enum class GAME_OBJECT {
  PLAYER,
  DOOR,
};

enum class OBJECT_TAG {
  SOLID,
};

void debugRect(SDL_Rect r);

class AbstractGameObject {
  public:
    collidable _collidable;
    v2 _position;
    velocity _velocity;
    gravity _gravity;

    virtual void init(SDL_Renderer* renderer);
    virtual void update(float dt);
    virtual void draw(SDL_Renderer* renderer, v2 offset);
    virtual ~AbstractGameObject() {
    }
    virtual void handleEvent(SDL_Event* event) {};
    void setListenForCollisions();
    virtual bool contains(SDL_Rect other);
    virtual bool contains(AbstractGameObject other);
    virtual void onCollision(AbstractGameObject* other) {}; // @TODO: connect this
    SDL_Rect getRect();
    SDL_Rect* getRectPointer();
    v2 getPosition();
    v2* getPositionPointer();
    bool getListensForCollisions();
    GAME_OBJECT getType();
    OBJECT_TAG getTag();
  protected:
    GAME_OBJECT _type;
    OBJECT_TAG _tag;
    /// Will be able to listen to other collisions.
    bool _listenForCollisions = false;
    Renderable _renderable;
};
