#include "player.h"
#include "../debugPrinter.h"
#include "../engine/inputHandler.h"
#include "damageNumbers.h"
#include "npc.h"
#include <cmath>
#include <cstdlib>

void obj::Player::init() {
  AbstractGameObject::init();

  _zIndex = 100;
  _tag = OBJECT_TAG::PLAYER;
  _type = GAME_OBJECT::PLAYER;
  _persist = true;

  this->_collidable.boundingBox = {18, 15, 18, 28};
  this->_collidable.rect = {0, 0, 0, 0};

  this->setupAnimations();

  this->jumpController.init(this);
  this->attackController.init(this);
  this->climbController.init(this);
  this->slideController.init(this);

  _normalGravity = _gravity.entityGravity;

  _interactableTexture =
      AssetManager::Instance()->getTexture("assets/sprites/interactable.png");

  // Bind key events
  event_register(EVENT_CODE_KEY_PRESSED, this, &obj::Player::onInputPressed);
  event_register(EVENT_CODE_KEY_RELEASED, this, &obj::Player::onInputReleased);
}

void obj::Player::setupAnimations() {
  SDL_Texture *texture = AssetManager::Instance()->getTexture(
      "assets/sprites/warrior/Warrior/SpriteSheet/Warrior_Sheet-Effect.png");

  int tw = 69;
  int th = 44;

  _renderable.spriteOffset = {15, 0};

  Animation *animIdle = new Animation(texture);
  Animation *animCrouch = new Animation(texture);
  Animation *animRun = new Animation(texture);
  Animation *animAttack = new Animation(texture, false);
  Animation *animAttack2 = new Animation(texture, false);
  Animation *animJumpAttack = new Animation(texture, false);
  Animation *animJump = new Animation(texture, false);
  Animation *animFall = new Animation(texture, false);
  Animation *animBackDash = new Animation(texture, false);
  Animation *animSlide = new Animation(texture, false);
  Animation *animUpToFall = new Animation(texture, false);
  Animation *animClimb = new Animation(texture);
  Animation *animDie = new Animation(texture, false);

  animIdle->addFrame({tw * 0, th * 0, tw, th}, 400);
  animIdle->addFrame({tw * 1, th * 0, tw, th}, 400);
  animIdle->addFrame({tw * 2, th * 0, tw, th}, 400);
  animIdle->addFrame({tw * 3, th * 0, tw, th}, 400);
  animIdle->addFrame({tw * 4, th * 0, tw, th}, 400);
  animIdle->addFrame({tw * 5, th * 0, tw, th}, 400);

  animCrouch->addFrame({tw * 4, th * 10, tw, th}, 500);
  animCrouch->addFrame({tw * 5, th * 10, tw, th}, 500);
  animCrouch->addFrame({tw * 0, th * 11, tw, th}, 500);
  animCrouch->addFrame({tw * 1, th * 11, tw, th}, 500);

  animRun->addFrame({tw * 0, th * 1, tw, th}, 100);
  animRun->addFrame({tw * 1, th * 1, tw, th}, 100);
  animRun->addFrame({tw * 2, th * 1, tw, th}, 100);
  animRun->addFrame({tw * 3, th * 1, tw, th}, 100);
  animRun->addFrame({tw * 4, th * 1, tw, th}, 100);
  animRun->addFrame({tw * 5, th * 1, tw, th}, 100);
  animRun->addFrame({tw * 0, th * 2, tw, th}, 100);
  animRun->addFrame({tw * 1, th * 2, tw, th}, 100);

  animAttack->addFrame({tw * 0, th * 3, tw, th}, 70);
  animAttack->addFrame({tw * 1, th * 3, tw, th}, 70);
  animAttack->addFrame({tw * 2, th * 3, tw, th}, 70);
  animAttack->addFrame({tw * 3, th * 3, tw, th}, 70);
  animAttack->addFrame({tw * 4, th * 3, tw, th}, 70);

  animAttack2->addFrame({tw * 5, th * 12, tw, th}, 70);
  animAttack2->addFrame({tw * 0, th * 13, tw, th}, 70);
  animAttack2->addFrame({tw * 1, th * 13, tw, th}, 70);
  animAttack2->addFrame({tw * 2, th * 13, tw, th}, 70);
  animAttack2->addFrame({tw * 3, th * 13, tw, th}, 50);

  animJumpAttack->addFrame({tw * 5, th * 12, tw, th}, 70);
  animJumpAttack->addFrame({tw * 0, th * 13, tw, th}, 70);
  animJumpAttack->addFrame({tw * 1, th * 13, tw, th}, 70);
  animJumpAttack->addFrame({tw * 2, th * 13, tw, th}, 70);
  animJumpAttack->addFrame({tw * 3, th * 13, tw, th}, 50);

  animJump->addFrame({tw * 5, th * 6, tw, th}, 100);
  animJump->addFrame({tw * 0, th * 7, tw, th}, 100);
  animJump->addFrame({tw * 1, th * 7, tw, th}, 100);

  animUpToFall->addFrame({tw * 2, th * 7, tw, th}, 100);
  animUpToFall->addFrame({tw * 3, th * 7, tw, th}, 100);

  animFall->addFrame({tw * 4, th * 7, tw, th}, 100);
  animFall->addFrame({tw * 5, th * 7, tw, th}, 100);
  animFall->addFrame({tw * 0, th * 8, tw, th}, 100);

  animBackDash->addFrame({tw * 0, th * 14, tw, th}, 100);

  animSlide->addFrame({tw * 2, th * 14, tw, th}, 100);
  animSlide->addFrame({tw * 3, th * 14, tw, th}, 100);
  animSlide->addFrame({tw * 4, th * 14, tw, th}, 100);
  animSlide->addFrame({tw * 5, th * 14, tw, th}, 100);

  animClimb->addFrame({tw * 1, th * 15, tw, th}, 100);
  animClimb->addFrame({tw * 2, th * 15, tw, th}, 100);
  animClimb->addFrame({tw * 2, th * 15, tw, th}, 100);
  animClimb->addFrame({tw * 4, th * 15, tw, th}, 100);
  animClimb->addFrame({tw * 5, th * 15, tw, th}, 100);
  animClimb->addFrame({tw * 0, th * 16, tw, th}, 100);
  animClimb->addFrame({tw * 1, th * 16, tw, th}, 100);
  animClimb->addFrame({tw * 2, th * 16, tw, th}, 100);

  animDie->addFrame({tw * 2, th * 4, tw, th}, 70);
  animDie->addFrame({tw * 3, th * 4, tw, th}, 70);
  animDie->addFrame({tw * 4, th * 4, tw, th}, 70);
  animDie->addFrame({tw * 5, th * 4, tw, th}, 70);
  animDie->addFrame({tw * 0, th * 5, tw, th}, 70);
  animDie->addFrame({tw * 1, th * 5, tw, th}, 70);
  animDie->addFrame({tw * 2, th * 5, tw, th}, 70);
  animDie->addFrame({tw * 3, th * 5, tw, th}, 70);
  animDie->addFrame({tw * 4, th * 5, tw, th}, 70);
  animDie->addFrame({tw * 5, th * 5, tw, th}, 70);
  animDie->addFrame({tw * 0, th * 6, tw, th}, 70);

  this->_animator = Animator();
  _animator.addAnimation("idle", animIdle);
  _animator.addAnimation("crouch", animCrouch);
  _animator.addAnimation("run", animRun);
  _animator.addAnimation("attack", animAttack);
  _animator.addAnimation("attack2", animAttack2);
  _animator.addAnimation("jumpAttack", animJumpAttack);
  _animator.addAnimation("jump", animJump);
  _animator.addAnimation("upToFall", animUpToFall);
  _animator.addAnimation("fall", animFall);
  _animator.addAnimation("backDash", animBackDash);
  _animator.addAnimation("slide", animSlide);
  _animator.addAnimation("climb", animClimb);
  _animator.addAnimation("die", animDie);

  _animator.setAnimation("idle");

  this->_renderable.texture = texture;
}

template <class T, class J>
Tile *obj::Player::tileAt(Rectangle<T, J> rect, std::string property) {
  auto tilesAbove = _collidable.tileExistsAtF(rect);

  for (TileExistsAtResponse &tile : tilesAbove) {
    if (property == "") {
      return &tile.tile;
    }

    auto tileset = &EntityManager::Instance()
                        ->getTilemap()
                        ->world->tilesetDefs[tile.tilesetId];

    if (tileset->tileHasTag(tile.tile.getTileId(), property)) {
      Layer *layer =
          &EntityManager::Instance()->getTilemap()->layers[tile.layerId];
      return &layer->tiles.at(tile.tileId);
    }
  }

  return nullptr;
}

void obj::Player::update(double dt) {
  InputHandler *inputHandler = InputHandler::Instance();

  debugFly = inputHandler->isHeld(BUTTON::DEBUG_FLY);

  if (state == State::DEAD) {
    hurt = false;
    _animator.setAnimation("die");
    _collidable.boundingBox = deadBoundingbox;

    if (!_animator.isPlaying()) {
      _animator.start();
    }

    _gravity.entityGravity = _normalGravity;
    _velocity.v.x = _velocity.calcFriction(_velocity.v.x, AIR_DEACCELERATION, dt);

    _collidable.moveAndSlide(&_position, &_velocity, dt);
    AbstractGameObject::update(dt);
    return;
  }

  if (debugFly) {
    _animator.setAnimation("idle");
    _velocity.v = { 0.0f, 0.0f };
    if (inputHandler->isHeld(BUTTON::LEFT)) {
      _position.x -= 0.5f * dt;
    }
    else if (inputHandler->isHeld(BUTTON::RIGHT)) {
      _position.x += 0.5f * dt;
    }

    if (inputHandler->isHeld(BUTTON::UP)) {
      _position.y -= 0.5f * dt;
    }
    else if (inputHandler->isHeld(BUTTON::DOWN)) {
      _position.y += 0.5f * dt;
    }

    AbstractGameObject::update(dt);

    return;
  }

  auto entitiesTouching = _collidable.objectExistsAt(_collidable.rect);

  if (state != State::DEAD && hp <= 0) {
    state = State::DEAD;
    return;
  }

  // Check hitbox
  if (!hurt) {
    for (auto other : entitiesTouching) {
      if (other->getType() == GAME_OBJECT::ENEMY && other->active) {
        int entityCenter = other->_collidable.rect.x - (other->_collidable.rect.w / 2);
        int center = _collidable.rect.x - (_collidable.rect.w / 2);
        if (center > entityCenter) {
          _velocity.v.x = +_hurtForce; 
          _velocity.v.y = -0.3f; 
        }
        else {
          _velocity.v.x = -_hurtForce; 
          _velocity.v.y = -0.3f; 
        }

        state = State::JUMP;
        hurt = true;
        hurtTimer.reset();
        hp -= other->damagePoints;
        break;
      }
    }
  }

  if (hurtTimer.elapsed() > _hurtInvicibility) {
    hurt = false;
  }

  _renderable.textureFlip =
      direction == Direction::LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  jumpController.update(dt);
  attackController.update(dt);

  Level *tilemap = EntityManager::Instance()->getTilemap();

  auto closeTiles = _collidable.tileExistsAtF({
      _collidable.rect.x - _collidable.rect.w,
      _collidable.rect.y - _collidable.rect.h,
      _collidable.rect.w * 3,
      _collidable.rect.h * 3,
  });

  onOneWayPlatform = false;
  if (onwayPlatformFallThroughTimer.elapsed() > ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW) {
    RectF feet = { _collidable.rect.x, _collidable.rect.y + _collidable.rect.h - 2, _collidable.rect.w, 2 };
    DebugPrinter::Instance()->addDebugRect(&feet, 0, 255, 0);
    if (_velocity.v.y >= 0.0f) {
      for(auto closeTile : closeTiles) {
        Tileset* tileset = &tilemap->world->tilesetDefs[closeTile.tilesetId];
        if (tileset->tileHasTag(closeTile.tile.getTileId(), "Oneway")) {
          float ct;
          v2f cn, cp;

          Rect closeTileRect = { closeTile.rect.x, closeTile.rect.y, closeTile.rect.w, 1 };

          if (_collidable.dynamicRectVsRect(&feet, _velocity, closeTileRect, cp, cn, ct, dt)) {
            if (cn.y < 0) {
              _position.y = closeTile.rect.y - 43;
              _velocity.v.y = 0.0f;
            }
          }
        }
      }
    }
  }

  RectF above = {
      _collidable.rect.x + 1,
      _position.y + normalBoundingbox.y - 1.0f,
      (float)_collidable.rect.w - 2,
      16,
  };

  if (tileAt(above, "Solid")) {
    hasTileAbove = true;
  } else {
    hasTileAbove = false;
  }

  isMoving = false;
  _canInteract = false;

  // Everything dependent on input within this if-case
  if (!hurt || hurtTimer.elapsed() > 250) {
    for (auto entity : entitiesTouching) {
      if (entity->canInteract && _gravity.onFloor) {
        _canInteract = true;

        if (inputHandler->isHeld(BUTTON::UP)) {
          state = State::IDLE;
          _animator.setAnimation("idle");
          ((obj::Npc *)entity)->onInteract(this);

          AbstractGameObject::update(dt);
          return;
        }
      }
    }

    if (climbController.update(dt)) {
      _gravity.entityGravity = 0.0f;
      AbstractGameObject::update(dt);
      _collidable.moveAndSlide(&_position, &_velocity, dt);
      return;
    }

    // Move left and right
    if (state != State::CROUCH && state != State::SLIDE) {
      if ((state != State::ATTACK || !_gravity.onFloor)) {
        if (inputHandler->isHeld(BUTTON::LEFT)) {
          direction = Direction::LEFT;
          _velocity.v.x -=
              (_gravity.onFloor ? RUN_ACCELERATION : AIR_ACCELERATION) * dt;
          if (state != State::ATTACK) {
            state = State::RUN;
          }
          isMoving = true;
        } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
          direction = Direction::RIGHT;
          _velocity.v.x +=
              (_gravity.onFloor ? RUN_ACCELERATION : AIR_ACCELERATION) * dt;
          if (state != State::ATTACK) {
            state = State::RUN;
          }
          isMoving = true;
        } else if (state != State::SLIDE && state != State::ATTACK &&
                  !hasTileAbove) {
          state = State::IDLE;
        }
      }
    }

    // Crouch
    if (inputHandler->isHeld(BUTTON::DOWN) &&
        onwayPlatformFallThroughTimer.elapsed() >
            ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW) {
      if (_gravity.onFloor) {
        if (state == State::IDLE || state == State::RUN) {
          if (state != State::CROUCH) {
            state = State::CROUCH;
          }
        }
      }
    } else if (state == State::CROUCH && !hasTileAbove) {
      state = State::IDLE;
    }

    if (state != State::ATTACK) {
      if (state == State::CROUCH) {
        _animator.setAnimation("crouch");
      } else if (state != State::SLIDE && state != State::CROUCH) {
        if (state != State::ATTACK && !_gravity.onFloor &&
            _velocity.v.y < -0.01f) {
          _animator.setAnimation("jump");
          if (_animator.getCurrent() == "jump" && _animator.hasPlayedThrough()) {
            _animator.setAnimation("upToFall");
          }
        } else if (!_gravity.onFloor && _velocity.v.y > 0.01f) {
          _animator.setAnimation("fall");
        } else {
          if (state == State::RUN) {
            _animator.setAnimation("run");
          } else {
            _animator.setAnimation("idle");
          }
        }
      }
    }

    this->slideController.update(dt);
  }

  // Cap velocity
  if (state != State::SLIDE) {
    if (_velocity.v.x >= RUN_SPEED) {
      _velocity.v.x = RUN_SPEED;
    } else if (_velocity.v.x <= -RUN_SPEED) {
      _velocity.v.x = -RUN_SPEED;
    }
  }


  Rect tilesBelowRect = {(int)round(_collidable.rect.x),
                         (int)_collidable.rect.bottom(),
                         (int)_collidable.rect.w, 1};

  previouslyOnFloor = _gravity.onFloor;
  _gravity.entityGravity = jumpController.jumpHold && _velocity.v.y < -0.3f
                               ? JUMP_SHORT_GRAVITY
                               : _normalGravity;

  if (!isMoving) {
    if (!_gravity.onFloor) {
      _velocity.v.x = _velocity.calcFriction(_velocity.v.x, AIR_DEACCELERATION, dt);
    } else if (state != State::SLIDE) {
      _velocity.v.x = _velocity.calcFriction(_velocity.v.x, RUN_DEACCELERATION, dt);
    }
  }

  if (state == State::SLIDE || state == State::CROUCH) {
    _collidable.boundingBox = crouchingBoundingbox;
  } else {
    _collidable.boundingBox = normalBoundingbox;
  }

  if (state == State::SLIDE || state == State::CROUCH) {
    _collidable.boundingBox.y = 27;
    _collidable.boundingBox.h = 16;
  } else {
    this->_collidable.boundingBox = {18, 15, 18, 28};
  }

  _prevPosition = _position;
  CollisionResponse resp = _collidable.moveAndSlide(&_position, &_velocity, dt);
  //resp.print();

  _gravity.onFloor = false;

  auto tilesBelow = _collidable.tileExistsAtI(tilesBelowRect);
  for (auto tile : tilesBelow) {
    Tileset* tileset = &tilemap->world->tilesetDefs[tile.tilesetId];
    bool oneWay = tileset->tileHasTag(tile.tile.getTileId(), "Oneway");

    if (oneWay && tile.rect.y != tilesBelowRect.bottom() - 1) {
      oneWay = false;
    }

    if (onwayPlatformFallThroughTimer.elapsed() <= ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW) {
      oneWay = false;
    }

    if (oneWay) {
      onOneWayPlatform = true;
    }

    if (tile.tile.getSolid() || oneWay) {
      _gravity.onFloor = true;
    }
  }

  AbstractGameObject::update(dt);
}

bool obj::Player::onInputPressed(u16 code, void* sender, void* listener_inst, event_context context) {
  Player* self = (Player*)listener_inst;
  u16 button = context.data.u16[0];

  if (self->state != State::DEAD && (!self->hurt || self->hurtTimer.elapsed() > 250)) {
    self->jumpController.onInputPressed(button);
    self->attackController.onInputPressed(button);
    self->slideController.onInputPressed(button);

    if (self->onOneWayPlatform && button == BUTTON::JUMP) {
      self->onwayPlatformFallThroughTimer.reset();
      self->onOneWayPlatform = false;
    }
  }

  return true;
};

bool obj::Player::onInputReleased(u16 code, void* sender, void* listener_inst, event_context context) {
  Player* self = (Player*)listener_inst;
  u16 button = context.data.u16[0];

  if (self->state != State::DEAD && (!self->hurt || self->hurtTimer.elapsed() > 250)) {
    self->jumpController.onInputReleased(button);
  }

  return true;
};

void obj::Player::draw(Renderer *renderer) {
  _renderable.textureRect = _animator.getFrame();
  _renderable.texture = _animator.getTexture();

  if (hurt) {
    _animator.setAnimation("backDash");
    _hurtFlashTimer += 0.1;
  }

  if (!hurt || (int)_hurtFlashTimer % 2) {
    // Draw player
    AbstractGameObject::draw(renderer);

    // Draw interactable
    if (_canInteract) {
      int yAdded = sin(_infiniteTimer.elapsed() / 100) * 2;
      Rect sr = {0, 0, 6, 14};
      RectF playerR = _collidable.addBoundingBox(_position);
      Rect dr = {(int)(playerR.x + playerR.w / 2) - 2 -
                    (direction == Direction::LEFT ? 1 : 0),
                (int)(playerR.y - 22) + yAdded, sr.w, sr.h};
      renderer->renderTexture(_interactableTexture, &sr, &dr, SDL_FLIP_NONE);
    }

    attackController.draw(renderer);
  }
}
