#pragma once

#include "../abstractGameobject.h"
#include "../engine/animator.h"
#include "../engine/renderer.h"
#include "../components/player/jump.h"
#include "../components/player/attack.h"
#include "../components/player/climb.h"
#include "../components/player/slide.h"

namespace obj {
enum class State {
  IDLE,
  CROUCH,
  RUN,
  JUMP,
  ATTACK,
  HURT,
  SLIDE,
  CLIMBING,
};

class Player : public AbstractGameObject {
public:
  virtual ~Player() {}
  void init() override;
  void setupAnimations();
  void update(double dt) override;

  template <class T, class J>
  Tile *tileAt(Rectangle<T, J> rect, std::string property);
  void onInputPressed(int button) override;
  void onInputReleased(int button) override;
  void draw(Renderer *renderer) override;
  float calcFriction(float v, float friction, double dt);
  State state = State::IDLE;
  bool hasTileAbove = false;
  Direction direction = Direction::RIGHT;
  Animator _animator;
  bool isMoving = false;
  bool onOneWayPlatform = false;

protected:
  /// How much higher the player should go if they hold the jump button (lower
  /// value means higher jump).
  float JUMP_SHORT_GRAVITY = 0.006f;
  /// How fast to get to max run speed
  const float RUN_ACCELERATION = 0.0030f;
  /// How fast to get to a stop
  const float RUN_DEACCELERATION = 0.002f;
  /// How fast to get to max run speed in air
  const float AIR_ACCELERATION = 0.01f;
  /// How fast to get to a stop in air
  const float AIR_DEACCELERATION = 0.0003f;
  /// Max run speed
  const float RUN_SPEED = 0.16f;

  /// When couching and pressing jump on top of a one-way platform we let the
  /// player go through it. This is how long we should act like it's not solid
  /// for.
  const uint32_t ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW = 100;

  Timer hurtTimer;
  Timer onwayPlatformFallThroughTimer;
  Timer _infiniteTimer; // Never to be reset. Just use for animations and such
  float _normalGravity;
  bool _canInteract = false;
  float backDashSpeed = 1.5f;
  bool isBackDashing = false;
  bool previouslyOnFloor = false;
  bool dead = false;
  Rect normalBoundingbox = {18, 15, 18, 28};
  Rect crouchingBoundingbox = {18, 27, 18, 16};
  SDL_Texture *_interactableTexture;
  JumpController jumpController;
  AttackController attackController;
  ClimbController climbController;
  SlideController slideController;
};
} // namespace obj
