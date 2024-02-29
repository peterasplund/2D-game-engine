#include "player.h"

float calcFriction(float v, float friction) {
  if (std::abs(v) > 0.01) {
    int sign = v > 0 ? 1 : -1;

    return v - friction * sign;
  }

  return 0.0f;
}

void obj::Player::init() {
  AbstractGameObject::init();

  _tag = OBJECT_TAG::PLAYER;

  this->_collidable.boundingBox = { 
    18, 15, 
    18, 28 
  };

  setListenForCollisions();

  SDL_Texture* texture = AssetManager::Instance()->getTexture("assets/sprites/warrior/Warrior/SpriteSheet/Warrior_Sheet-Effect.png");

  int tw = 69;
  int th = 44;

  _renderable.spriteOffset = { 15, 0 };

  Animation* animIdle = new Animation(texture);
  Animation* animCrouch = new Animation(texture);
  Animation* animRun = new Animation(texture);
  Animation* animAttack = new Animation(texture, false);
  Animation* animAttack2 = new Animation(texture, false);
  Animation* animJumpAttack = new Animation(texture, false);
  Animation* animJump = new Animation(texture, false);
  Animation* animFall = new Animation(texture, false);
  Animation* animBackDash = new Animation(texture, false);
  Animation* animSlide = new Animation(texture, false);
  Animation* animUpToFall = new Animation(texture, false);
  Animation* animClimb = new Animation(texture);
  Animation* animDie = new Animation(texture, false);

  animIdle->addFrame({ tw * 0, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 1, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 2, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 3, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 4, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 5, th * 0, tw, th }, 500);

  animCrouch->addFrame({ tw * 4, th * 10, tw, th }, 500);
  animCrouch->addFrame({ tw * 5, th * 10, tw, th }, 500);
  animCrouch->addFrame({ tw * 0, th * 11, tw, th }, 500);
  animCrouch->addFrame({ tw * 1, th * 11, tw, th }, 500);

  animRun->addFrame({ tw * 0, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 1, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 2, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 3, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 4, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 5, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 0, th * 2, tw, th }, 100);
  animRun->addFrame({ tw * 1, th * 2, tw, th }, 100);

  animAttack->addFrame({ tw * 0, th * 3, tw, th }, 70);
  animAttack->addFrame({ tw * 1, th * 3, tw, th }, 70);
  animAttack->addFrame({ tw * 2, th * 3, tw, th }, 70);
  animAttack->addFrame({ tw * 3, th * 3, tw, th }, 70);
  animAttack->addFrame({ tw * 4, th * 3, tw, th }, 70);

  animAttack2->addFrame({ tw * 5, th * 12, tw, th }, 70);
  animAttack2->addFrame({ tw * 0, th * 13, tw, th }, 70);
  animAttack2->addFrame({ tw * 1, th * 13, tw, th }, 70);
  animAttack2->addFrame({ tw * 2, th * 13, tw, th }, 70);
  animAttack2->addFrame({ tw * 3, th * 13, tw, th }, 50);

  animJumpAttack->addFrame({ tw * 5, th * 12, tw, th }, 70);
  animJumpAttack->addFrame({ tw * 0, th * 13, tw, th }, 70);
  animJumpAttack->addFrame({ tw * 1, th * 13, tw, th }, 70);
  animJumpAttack->addFrame({ tw * 2, th * 13, tw, th }, 70);
  animJumpAttack->addFrame({ tw * 3, th * 13, tw, th }, 50);

  animJump->addFrame({ tw * 5, th * 6, tw, th }, 100);
  animJump->addFrame({ tw * 0, th * 7, tw, th }, 100);
  animJump->addFrame({ tw * 1, th * 7, tw, th }, 100);

  animUpToFall->addFrame({ tw * 2, th * 7, tw, th }, 100);
  animUpToFall->addFrame({ tw * 3, th * 7, tw, th }, 100);

  animFall->addFrame({ tw * 4, th * 7, tw, th }, 100);
  animFall->addFrame({ tw * 5, th * 7, tw, th }, 100);
  animFall->addFrame({ tw * 0, th * 8, tw, th }, 100);

  animBackDash->addFrame({ tw * 0, th * 14, tw, th }, 100);

  animSlide->addFrame({ tw * 2, th * 14, tw, th }, 100);
  animSlide->addFrame({ tw * 3, th * 14, tw, th }, 100);
  animSlide->addFrame({ tw * 4, th * 14, tw, th }, 100);
  animSlide->addFrame({ tw * 5, th * 14, tw, th }, 100);

  animClimb->addFrame({ tw * 1, th * 15, tw, th }, 100);
  animClimb->addFrame({ tw * 2, th * 15, tw, th }, 100);
  animClimb->addFrame({ tw * 2, th * 15, tw, th }, 100);
  animClimb->addFrame({ tw * 4, th * 15, tw, th }, 100);
  animClimb->addFrame({ tw * 5, th * 15, tw, th }, 100);
  animClimb->addFrame({ tw * 0, th * 16, tw, th }, 100);
  animClimb->addFrame({ tw * 1, th * 16, tw, th }, 100);
  animClimb->addFrame({ tw * 2, th * 16, tw, th }, 100);

  animDie->addFrame({ tw * 2, th * 4, tw, th }, 70);
  animDie->addFrame({ tw * 3, th * 4, tw, th }, 70);
  animDie->addFrame({ tw * 4, th * 4, tw, th }, 70);
  animDie->addFrame({ tw * 5, th * 4, tw, th }, 70);
  animDie->addFrame({ tw * 0, th * 5, tw, th }, 70);
  animDie->addFrame({ tw * 1, th * 5, tw, th }, 70);
  animDie->addFrame({ tw * 2, th * 5, tw, th }, 70);
  animDie->addFrame({ tw * 3, th * 5, tw, th }, 70);
  animDie->addFrame({ tw * 4, th * 5, tw, th }, 70);
  animDie->addFrame({ tw * 5, th * 5, tw, th }, 70);
  animDie->addFrame({ tw * 0, th * 6, tw, th }, 70);

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
  
  _normalGravity = _gravity.entityGravity;
}

TileData* obj::Player::tileAt(RectF rect, std::string property = "") {
  auto tilesAbove = _collidable.tileExistsAt(rect);

  if (tilesAbove.size() > 0) {
    for(auto tile : tilesAbove) {
      TileData* tileData = EntityManager::Instance()->getTilemap()->getTileData(tile.tileId);
      if (property == "") {
        return tileData;
      }

      auto it = tileData->propertiesBool.find(property);
      if (it != tileData->propertiesBool.end()) {
        if (it->second) {
          return tileData;
        }
      }
    }
  }

  return nullptr;
}

void obj::Player::update(float dt) {
  AbstractGameObject::update(dt);
  InputHandler* inputHandler = InputHandler::Instance();
  isMoving = false;

  if (dead) {
    _animator.setAnimation("die");
    if (!_animator.isPlaying()) {
      _animator.start();
    }
    return;
  }

  TileData* ladderAbove = tileAt({
    round(_collidable.rect.x),
    _collidable.rect.y - _collidable.rect.h / 2,
    floor(_collidable.rect.w),
    _collidable.rect.h / 2
  }, "ladder");

  if (state == State::CLIMBING) {
    _velocity.v.y = -0.0f;
    _renderable.textureFlip = SDL_FLIP_NONE;
    if (InputHandler::Instance()->isHeld(BUTTON::UP)) {
      _velocity.v.y = -0.1f;
    }
    else if (InputHandler::Instance()->isHeld(BUTTON::DOWN)) {
      _velocity.v.y = 0.1f;
    }

    _velocity.v.x = 0.0f;

    if (!ladderAbove) {
      _animator.setAnimation("crouch");
    }
    else {
      _animator.setAnimation("climb");
      _animator.stop();
    }

    if (_velocity.v.y < -0.001f) {
      if (!_animator.isPlaying()) {
        _animator.start();
      }
    }
    else if (_velocity.v.y > 0.001f) {
      if (!_animator.isPlaying()) {
        _animator.start();
      }
      // @TODO: reverse animation
    }


    //_collidable.update(_position);
    auto resp = _collidable.moveAndSlide(&_position, &_velocity, dt);

    if (resp.bottom) {
      state = State::IDLE;
    }

    _velocity.v = {0,0};

    auto tilesWithin = _collidable.tileExistsAt({
      round(_collidable.rect.x),
      _collidable.rect.y,
      floor(_collidable.rect.w),
      _collidable.rect.h
    });

    TileData* onLadder = tileAt({
      round(_collidable.rect.x),
      _collidable.rect.y,
      floor(_collidable.rect.w),
      _collidable.rect.h
    }, "ladder");

    if (!onLadder) {
      state = State::IDLE;
    }

    return;
  }

  if (state != State::ATTACK && state != State::CROUCH && state != State::SLIDE) {
    if (inputHandler->isHeld(BUTTON::LEFT)) {
      _velocity.v.x -= _gravity.onFloor ? RUN_ACCELERATION : AIR_ACCELERATION;
      state = State::RUN;
      isMoving = true;
    } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
      _velocity.v.x += _gravity.onFloor ? RUN_ACCELERATION : AIR_ACCELERATION;
      state = State::RUN;
      isMoving = true;
    } else if (state != State::SLIDE) {
      state = State::IDLE;
    }
  }

  // Crouch
  if (inputHandler->isHeld(BUTTON::DOWN) && onwayPlatformFallThroughTimer.elapsed() > ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW) {
    if (_gravity.onFloor) {
      if (state == State::IDLE || state == State::RUN) {
        if (state != State::CROUCH) {
          state = State::CROUCH;
        }
      }
    }
  } else if (state == State::CROUCH) {
    state = State::IDLE;
  }

  if (state != State::SLIDE) {
    if (_velocity.v.x >= RUN_SPEED) {
      _velocity.v.x = RUN_SPEED;
    }
    else if (_velocity.v.x <= -RUN_SPEED) {
      _velocity.v.x = -RUN_SPEED;
    }
  }

  if (state == State::CROUCH) {
    _animator.setAnimation("crouch");
  }
  else if (state != State::SLIDE) {
    if (state == State::ATTACK) {
        if (_gravity.onFloor) {
          _animator.setAnimation("attack");
        }
        else {
          _animator.setAnimation("jumpAttack");
        }
    }
    else if (!_gravity.onFloor && _velocity.v.y < -0.01f) {
        _animator.setAnimation("jump");
        if (_animator.getCurrent() == "jump" && _animator.hasPlayedThrough()) {
          _animator.setAnimation("upToFall");
        }
    }
    else if (!_gravity.onFloor && _velocity.v.y > 0.01f) {
        _animator.setAnimation("fall");
    }
    else {
      if (state == State::RUN) {
        _animator.setAnimation("run");
      }
      else {
        _animator.setAnimation("idle");
      }
    }
  }

  if (_velocity.v.x > 0.01f) {
    direction = "right";
  }
  else if (_velocity.v.x < -0.01f) {
    direction = "left";
  }

  _renderable.textureFlip = direction == "left" ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  // End attack
  if (attackTimer.elapsed() > attackDelay && state == State::ATTACK) {
    state = State::IDLE;
    _animator.reset();
    _animator.setAnimation("idle");
  }

  // End slide
  if (slideTimer.elapsed() > slideDelay && state == State::SLIDE) {
    state = State::IDLE;
    _animator.reset();
    _animator.setAnimation("idle");
  }

  if (state == State::SLIDE) {
    if (_animator.getCurrent() != "slide") {
      _animator.reset();
      _animator.setAnimation("slide");
      _velocity.v.x += direction == "left" ? -SLIDE_POWER : SLIDE_POWER;
    }
  }


  // die and respawn when falling of level
  if (_position.y > 370.0f) {
    _position.y = -(float)_renderable.textureRect.h;
  }

  auto resp = _collidable.moveAndSlide(&_position, &_velocity, dt);
  
  if (resp.hasCollision()) {
    // resp.print();
  }

  auto tilesWithin = _collidable.tileExistsAt({
    round(_collidable.rect.x),
    _collidable.rect.y,
    floor(_collidable.rect.w),
    _collidable.rect.h
  });

  if (_position.x > 200) {
      dead = true;
  }

  // Climb onto ladder
  if (tilesWithin.size() > 0) {
    for(auto tile : tilesWithin) {
      int tileCenter = tile.rect.x + tile.rect.w / 2;
      int playerCenter = _collidable.rect.x + _collidable.rect.w / 2;
      if (playerCenter > tileCenter - LADDER_X_DEADZONE && playerCenter < tileCenter + LADDER_X_DEADZONE) { 
        if (inputHandler->isHeld(BUTTON::UP)) {
          TileData* tileData = EntityManager::Instance()->getTilemap()->getTileData(tile.tileId);
          if (tileData->propertiesBool.find("ladder") != tileData->propertiesBool.end()) {
            if (tileData->propertiesBool.at("ladder")) {
              // @TODO: something is wrong with the tile rect or something
              _position.x = tile.rect.x - 23;
              //_position.y = tile.rect.y - _collidable.rect.y;
              state = State::CLIMBING;
              return;
            }
          }
        }
      }
    }
  }


  auto tilesBelow = _collidable.tileExistsAt({
    round(_collidable.rect.x),
    (_collidable.rect.y + _collidable.rect.h + 1),
    floor(_collidable.rect.w),
    1
  });

  _gravity.onFloor = false;
  onOneWayPlatform = false;
  if (tilesBelow.size() > 0) {
    for(auto tile : tilesBelow) {
      TileData* tileData = EntityManager::Instance()->getTilemap()->getTileData(tile.tileId);

      if (tileData->solid) {
        _jumpHold = false;
        _gravity.onFloor = true;
      }
      else {
        if (onwayPlatformFallThroughTimer.elapsed() > ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW) {
          if (tileData->propertiesBool.find("oneway") != tileData->propertiesBool.end()) {
            if (tileData->propertiesBool.at("oneway") && _velocity.v.y >= 0.f && _collidable.rect.bottom() <= tile.rect.y + ONEWAY_PLATFORM_GRACE) {
              onOneWayPlatform = true;
              _jumpHold = false;
              _gravity.onFloor = true;
              _velocity.v.y = 0.0f;
              _position.y = tile.rect.y - _collidable.rect.h - _collidable.boundingBox.y;
            }
          }
        }
      }
    }
  }

  previouslyOnFloor = _gravity.onFloor;

  auto &v = _velocity;
  auto &p = _position;

  _gravity.entityGravity = _jumpHold && _velocity.v.y < -0.3f ? JUMP_SHORT_GRAVITY : _normalGravity;

  if (!isMoving) {
    if (!_gravity.onFloor) {
      _velocity.v.x = calcFriction(_velocity.v.x, AIR_DEACCELERATION);
    }
    else if (state == State::SLIDE) {
      _velocity.v.x = calcFriction(_velocity.v.x, SLIDE_DEACCELERATION);
    }
    else {
      _velocity.v.x = calcFriction(_velocity.v.x, RUN_DEACCELERATION);
    }
  }

  // Jump buffer
  if (jumpBufferTimer.elapsed() < JUMP_BUFFER_WINDOW && _gravity.onFloor && inputHandler->isHeld(BUTTON::JUMP)) {
    if (state == State::IDLE || state == State::RUN) {
      performJump();
    }
  }

  if (_justJumped) {
    _jumpHold = true;
    _justJumped = false;
  }
}

void obj::Player::performJump() {
  jumpBuffered = false;
  state = State::JUMP;
  _velocity.v.y = -JUMP_POWER;
    _jumpHold = true;
  _justJumped = true;
}

void obj::Player::jump() {
  if (state != State::ATTACK && state != State::CROUCH) {
    if (_gravity.onFloor || state == State::SLIDE || state == State::CLIMBING) {
      performJump();
    }
    if (!_gravity.onFloor) {
      jumpBufferTimer.reset();
      jumpBuffered = true;
    }
  }
}

void obj::Player::attack() {
  if (attackTimer.elapsed() > attackDelay/* && g.onFloor*/) {
    state = State::ATTACK;
    _animator.reset();
    _animator.setAnimation("attack");
    attackTimer.reset();
  }
}

void obj::Player::slide() {
  if (state == State::CLIMBING) {
    return;
  }

  // Slide
  if (state == State::CROUCH) {
    // Check if on top of one-way platform. In that case we don't wanna slide 
    // but deactivate the solid of the platform until we're below it
    if (onOneWayPlatform) {
      onwayPlatformFallThroughTimer.reset();
      state = State::JUMP;
    }
    else {
      state = State::SLIDE;
      slideTimer.reset();
    }
  }
}

void obj::Player::onInputPressed(int button) {
  if (button == BUTTON::JUMP) {
    if (state == State::CROUCH) {
      slide();
    }
    else {
      jump();
    }
  }
  if (button == BUTTON::ATTACK) attack();
};

void obj::Player::onInputReleased(int button) {
  if (button == BUTTON::JUMP) {
    _jumpHold = false;
  }
};

void obj::Player::draw(Renderer* renderer) {
  _renderable.textureRect = _animator.getFrame();
  _renderable.texture = _animator.getTexture();

  AbstractGameObject::draw(renderer);
}
