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
  Animation* animRun = new Animation(texture);
  Animation* animAttack = new Animation(texture, false);
  Animation* animJump = new Animation(texture, false);
  Animation* animFall = new Animation(texture, false);
  Animation* animBackDash = new Animation(texture, false);
  Animation* animSlide = new Animation(texture, false);
  Animation* animUpToFall = new Animation(texture, false);

  animIdle->addFrame({ tw * 0, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 1, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 2, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 3, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 4, th * 0, tw, th }, 500);
  animIdle->addFrame({ tw * 5, th * 0, tw, th }, 500);

  animRun->addFrame({ tw * 0, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 1, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 2, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 3, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 4, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 5, th * 1, tw, th }, 100);
  animRun->addFrame({ tw * 0, th * 2, tw, th }, 100);
  animRun->addFrame({ tw * 1, th * 2, tw, th }, 100);

  animAttack->addFrame({ tw * 0, th * 3, tw, th }, 100);
  animAttack->addFrame({ tw * 1, th * 3, tw, th }, 100);
  animAttack->addFrame({ tw * 2, th * 3, tw, th }, 100);
  animAttack->addFrame({ tw * 3, th * 3, tw, th }, 100);
  animAttack->addFrame({ tw * 4, th * 3, tw, th }, 100);

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

  this->_animator = Animator();
  _animator.addAnimation("idle", animIdle);
  _animator.addAnimation("run", animRun);
  _animator.addAnimation("attack", animAttack);
  _animator.addAnimation("jump", animJump);
  _animator.addAnimation("upToFall", animUpToFall);
  _animator.addAnimation("fall", animFall);
  _animator.addAnimation("backDash", animBackDash);
  _animator.addAnimation("slide", animSlide);
  
  _animator.setAnimation("idle");

  this->_renderable.texture = texture;
  
  _normalGravity = _gravity.entityGravity;
}

void obj::Player::update(float dt) {
  AbstractGameObject::update(dt);
  InputHandler* inputHandler = InputHandler::Instance();

if (state == State::SLIDE) {
  if (_animator.getCurrent() != "slide") {
    _animator.reset();
    _animator.setAnimation("slide");
    _velocity.v.x += direction == "left" ? -0.6 : 0.6;
  }
}

if (state != State::ATTACK) {
  if (state != State::SLIDE) {
    if (inputHandler->isHeld(BUTTON::LEFT)) {
      direction = "left";
      _velocity.v.x -= RUN_ACCELERATION;
      _renderable.textureFlip = SDL_FLIP_HORIZONTAL;
      state = State::RUN;
    } else if (inputHandler->isHeld(BUTTON::RIGHT)) {
      direction = "right";
      _velocity.v.x += RUN_ACCELERATION;
      _renderable.textureFlip = SDL_FLIP_NONE;
      state = State::RUN;
    } else if (state != State::SLIDE) {
      state = State::IDLE;
    }
  }
}

if (_velocity.v.x >= RUN_SPEED) {
  _velocity.v.x = RUN_SPEED;
}
else if (_velocity.v.x <= -RUN_SPEED) {
  _velocity.v.x = -RUN_SPEED;
}

if (state != State::SLIDE) {
  if (state == State::ATTACK) {
      _animator.setAnimation("attack");
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

// End ttack
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

// Slide
if (inputHandler->isHeld(BUTTON::DOWN) && _gravity.onFloor) {
  state = State::SLIDE;
  slideTimer.reset();
}

  // die and respawn when falling of level
  if (_position.y > 370.0f) {
    _position.y = -(float)_renderable.textureRect.h;
  }

  auto resp = _collidable.moveAndSlide(&_position, &_velocity, dt);
  
  if (resp.hasCollision()) {
    // resp.print();
  }

  auto tilesBelow = _collidable.tileExistsAt({
    round(_collidable.rect.x),
    (_collidable.rect.y + _collidable.rect.h + 1),
    floor(_collidable.rect.w),
    1
  });


  _gravity.onFloor = false;
  if (tilesBelow.size() > 0) {
    for(auto tile : tilesBelow) {
      TileData* tileData = EntityManager::Instance()->getTilemap()->getTileData(tile.tileId);

      if (tileData->solid) {
        _jumpHold = false;
        _gravity.onFloor = true;
      }
      else {
        if (tileData->propertiesBool.find("oneway") != tileData->propertiesBool.end()) {
          if (tileData->propertiesBool.at("oneway") && _velocity.v.y >= 0.f && _collidable.rect.bottom() <= tile.rect.y + ONEWAY_PLATFORM_GRACE) {
            _jumpHold = false;
            _gravity.onFloor = true;
            _velocity.v.y = 0.0f;
            _position.y = tile.rect.y - _collidable.rect.h - _collidable.boundingBox.y;
          }
        }
      }
    }
  }

  previouslyOnFloor = _gravity.onFloor;

  auto &v = _velocity;
  auto &p = _position;

  _gravity.entityGravity = _jumpHold && _velocity.v.y < -0.3f ? JUMP_SHORT_GRAVITY : _normalGravity;

  if (!inputHandler->isHeld(BUTTON::LEFT) && !inputHandler->isHeld(BUTTON::RIGHT)) {
    _velocity.v.x = calcFriction(_velocity.v.x, RUN_DEACCELERATION);
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
    if (state != State::ATTACK) {
      if (_gravity.onFloor || state == State::SLIDE) {
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

void obj::Player::onInputPressed(int button) {
  if (button == BUTTON::JUMP)   jump();
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
