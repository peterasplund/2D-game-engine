#include "player.h"
#include "../debugPrinter.h"
#include "../engine/inputHandler.h"
#include "damageNumbers.h"
#include "npc.h"
#include <cmath>
#include <cstdlib>

float obj::Player::calcFriction(float v, float friction, double dt) {
  if (v < 0) {
    return std::min<float>(0.0f, v + (friction * dt));
  }
  else if (v > 0) {
    return std::max<float>(0.0f, v - (friction * dt));
  }
  /*

  if (std::abs(v) > 0.01) {
    int sign = v > 0 ? 1 : -1;

    return (v - (friction * dt) * sign);
  }
  */

  return 0.0f;
}

void obj::Player::init() {
  AbstractGameObject::init();

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
  if (_velocity.v.x > 0.01f) {
    direction = Direction::RIGHT;
  } else if (_velocity.v.x < -0.01f) {
    direction = Direction::LEFT;
  }

  _renderable.textureFlip =
      direction == Direction::LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  jumpController.update(dt);
  attackController.update(dt);

  // Level *tilemap = EntityManager::Instance()->getTilemap();
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

  InputHandler *inputHandler = InputHandler::Instance();
  isMoving = false;

  auto entitiesTouching = _collidable.objectExistsAt(_collidable.rect);

  _canInteract = false;
  for (auto entity : entitiesTouching) {
    if (entity->getType() == GAME_OBJECT::NPC && _gravity.onFloor) {
      _canInteract = true;

      if (inputHandler->isHeld(BUTTON::UP)) {
        ((obj::Npc *)entity)->talk();

        AbstractGameObject::update(dt);
        return;
      }
    }
  }

  if (dead) {
    _animator.setAnimation("die");
    if (!_animator.isPlaying()) {
      _animator.start();
    }

    AbstractGameObject::update(dt);
    return;
  }

  if (climbController.update(dt)) {
    AbstractGameObject::update(dt);
    _collidable.moveAndSlide(&_position, &_velocity, dt);
    return;
  }

  if (state != State::CROUCH && state != State::SLIDE) {
    if ((state != State::ATTACK || !_gravity.onFloor)) {
      if (inputHandler->isHeld(BUTTON::LEFT)) {
        _velocity.v.x -=
            (_gravity.onFloor ? RUN_ACCELERATION : AIR_ACCELERATION) * dt;
        if (state != State::ATTACK) {
          state = State::RUN;
        }
        isMoving = true;
      } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
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

  if (state != State::SLIDE) {
    if (_velocity.v.x >= RUN_SPEED) {
      _velocity.v.x = RUN_SPEED;
    } else if (_velocity.v.x <= -RUN_SPEED) {
      _velocity.v.x = -RUN_SPEED;
    }
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

  onOneWayPlatform = false;
  Rect tilesBelowRect = {(int)round(_collidable.rect.x),
                         (int)_collidable.rect.bottom(),
                         (int)_collidable.rect.w, 1};
  auto tilesBelow = _collidable.tileExistsAtI(tilesBelowRect);
  /*

  auto closeTiles = _collidable.tileExistsAtF({
      _collidable.rect.x - _collidable.rect.w,
      _collidable.rect.y - _collidable.rect.h,
      _collidable.rect.w * 3,
      _collidable.rect.h * 3,
  });

  //_gravity.onFloor = false;
  RectF pxBelow = _collidable.rect;
  pxBelow.h = _collidable.rect.h + 1;

  float prevBottom = _prevPosition.y + _collidable.boundingBox.bottom();
  float newBottom = _position.y + _collidable.boundingBox.bottom();
  // printf("prev: %f new: %f\n", prevBottom, newBottom);

  for (auto tile : tilesBelow) {
    Tileset *tileset = &tilemap->world->tilesetDefs[tile.tilesetId];
    if (tileset->tileHasTag(tile.tile.getTileId(), "Oneway")) {
      // bool standingOnPlatform = (int)_collidable.rect.bottom() + 1 ==
      // (int)tile.rect.top();
      bool passedThroughThisFrame =
          prevBottom < tile.rect.y && newBottom >= tile.rect.y;
      printf("%d\n", passedThroughThisFrame);

      if (passedThroughThisFrame) {
        if (tile.rect.hasIntersection(&tilesBelowRect) &&
            _velocity.v.y > 0.1f) {
          // if ((pxBelow.hasIntersection(&tile.rect) && _velocity.v.y == 0.0f))
          // {
          printf("on platform\n");
          onOneWayPlatform = true;
          _gravity.onFloor = true;
          _velocity.v.y = 0.0f;
          _position.y =
              tile.rect.y - _collidable.rect.h - _collidable.boundingBox.y;
          break;
        } else if (_collidable.rect.bottom() == tile.rect.top() - 1) {
          onOneWayPlatform = true;
          _gravity.onFloor = true;
          _velocity.v.y = 0.0f;
          _position.y =
              tile.rect.y - _collidable.rect.h - _collidable.boundingBox.y;
          break;
        }
      }
    }
  }
  */

  // SECTION: handle onFloor by checking below player.
  /*
  auto tilesBelow = _collidable.tileExistsAt<float>(
      {_collidable.rect.x, (_collidable.rect.y + _collidable.rect.h),
       _collidable.rect.w, 1});

  if (tilesBelow.size() > 0) {
    for (auto tile : tilesBelow) {
      Tileset* tileset = &tilemap->world->tilesetDefs[tile.tilesetId];

      if (tile.tile.getSolid() || tileset->tileHasTag(tile.tile.getTileId(),
  "Oneway")) { _gravity.onFloor = true;
      }
    }
  }
  */

  /*
    else {
    Tileset* tileset = &tilemap->world->tilesetDefs[tile.tilesetId];

      if (onwayPlatformFallThroughTimer.elapsed() >
    ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW) { if
    (tileset->tileHasTag(tile.tile.getTileId(), "Oneway")) { if (prevBB.bottom()
    <= _collidable.rect.top()) { printf("on top\n"); onOneWayPlatform = true;
            _jumpHold = false;
            _gravity.onFloor = true;
            _velocity.v.y = 0.0f;
            setPosition({_position.x, tile.rect.y - _collidable.rect.h -
                                          _collidable.boundingBox.y});
          }
        }
      }
    }
  */

  previouslyOnFloor = _gravity.onFloor;
  _gravity.entityGravity = jumpController.jumpHold && _velocity.v.y < -0.3f
                               ? JUMP_SHORT_GRAVITY
                               : _normalGravity;

  if (!isMoving) {
    if (!_gravity.onFloor) {
      _velocity.v.x = calcFriction(_velocity.v.x, AIR_DEACCELERATION, dt);
    } else if (state != State::SLIDE) {
      _velocity.v.x = calcFriction(_velocity.v.x, RUN_DEACCELERATION, dt);
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
  _gravity.onFloor = false;
  _collidable.moveAndSlide(&_position, &_velocity, dt);

  for (auto tile : tilesBelow) {
    if (tile.tile.getSolid()) {
      _gravity.onFloor = true;
    }
  }

  AbstractGameObject::update(dt);
}

void obj::Player::onInputPressed(int button) {
  jumpController.onInputPressed(button);
  attackController.onInputPressed(button);
  slideController.onInputPressed(button);
};

void obj::Player::onInputReleased(int button) {
  jumpController.onInputReleased(button);
};

void obj::Player::draw(Renderer *renderer) {
  _renderable.textureRect = _animator.getFrame();
  _renderable.texture = _animator.getTexture();
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
