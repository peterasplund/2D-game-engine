#pragma once

#include "components/renderable.h"
#include "components/velocity.h"
#include "components/gravity.h"
#include "components/collidable.h"
#include "globals.h"
#include "math.h"

class AbstractGameObject {
  public:
    collidable _collidable;
    v2f _position;
    velocity _velocity;
    gravity _gravity;

    virtual void init();
    virtual void update(float dt);
    virtual void draw(SDL_Renderer* renderer, v2f offset);
    virtual ~AbstractGameObject() {
    }
    virtual void handleEvent(SDL_Event* event) {};
    virtual void onInputPressed(int button) {};
    virtual void onInputReleased(int button) {};
    void setListenForCollisions();
    virtual bool contains(RectF other);
    virtual bool contains(AbstractGameObject other);
    virtual void onCollision(AbstractGameObject* other) {}; // @TODO: connect this
    Rect getRect();
    RectF getRectFloat();
    RectF* getRectPointer();
    v2f getPosition();
    v2f* getPositionPointer();
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
