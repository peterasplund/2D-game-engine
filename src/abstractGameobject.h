#pragma once

#include "components/renderable.h"
#include "components/velocity.h"
#include "components/gravity.h"
#include "components/collidable.h"
#include "globals.h"
#include "renderer.h"
#include "math.h"

class AbstractGameObject {
  public:
    collidable _collidable;
    v2f _position;
    velocity _velocity;
    gravity _gravity;
    bool _persist = false;
    bool dead = false;

    virtual void init();
    virtual void update(float dt);
    virtual void draw(Renderer* renderer);
    virtual ~AbstractGameObject() {
    }
    virtual void handleEvent(SDL_Event* event) {};
    virtual void onInputPressed(int button) {};
    virtual void onInputReleased(int button) {};
    void setListenForCollisions();
    virtual bool contains(RectF other);
    virtual bool contains(AbstractGameObject other);
    virtual void onCollision(AbstractGameObject* other) {}; // @TODO: connect this
    virtual void damage(int dmg) {
      printf("base damage!\n");
    };
    Rect getRect();
    RectF getRectFloat();
    RectF* getRectPointer();
    Rect getTextureRect() {
      return {
        (int)round(_position.x),
        (int)round(_position.y),
        _renderable.textureRect.w,
        _renderable.textureRect.h,
      };
    }
    v2f getPosition();
    void setPosition(v2f p);
    v2f* getPositionPointer();
    bool getListensForCollisions();
    GAME_OBJECT getType();
    OBJECT_TAG getTag();
    bool hurt = false;
  protected:
    GAME_OBJECT _type;
    OBJECT_TAG _tag;
    /// Will be able to listen to other collisions.
    bool _listenForCollisions = false;
    Renderable _renderable;
};
