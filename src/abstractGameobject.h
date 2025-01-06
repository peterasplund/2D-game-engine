#pragma once

#include "components/collidable.h"
#include "components/gravity.h"
#include "components/renderable.h"
#include "components/velocity.h"
#include "engine/math.h"
#include "engine/renderer.h"
#include "globals.h"
#include <SDL2/SDL_events.h>

namespace obj {
  class Player;
};

enum class Direction { LEFT, RIGHT };

class AbstractGameObject {
public:
  collidable _collidable;
  v2f _position;
  v2f _prevPosition;
  velocity _velocity;
  gravity _gravity;
  bool _persist = false;
  bool dead = false;
  int _zIndex = 0;
  int damagePoints = 5;
  bool canInteract = false;

  virtual void init();
  virtual void update(double dt);
  virtual void draw(Renderer *renderer);
  virtual ~AbstractGameObject() {}
  virtual void handleEvent(SDL_Event *event) {};
  virtual void onInputPressed(int button) {};
  virtual void onInputReleased(int button) {};
  void setListenForCollisions();
  virtual bool contains(RectF other);
  virtual bool contains(AbstractGameObject other);
  virtual void onInteract(obj::Player *other) {};
  virtual void onCollision(AbstractGameObject *other) {}; // @TODO: connect this
  virtual void damage(int dmg) { printf("base damage!\n"); };
  Rect getRect();
  RectF getRectFloat();
  RectF *getRectPointer();
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
  v2f *getPositionPointer();
  GAME_OBJECT getType();
  OBJECT_TAG getTag();
  bool hurt = false;
  bool invincible = false;
  bool active = true;
  Renderable _renderable;

protected:
  GAME_OBJECT _type;
  OBJECT_TAG _tag;
};
