#include "slide.h"
#include "../../engine/inputHandler.h"
#include "../../obj/player.h"

void SlideController::update(double dt) {
  obj::State s = player->state;

  // End slide when timer has elapsed
  if (slideTimer.elapsed() > slideDelay && s == obj::State::SLIDE) {
    player->state = obj::State::CROUCH;
    player->_animator.reset();
  }

  // Decelerate
  if (!player->isMoving) {
    if (player->_gravity.onFloor && s == obj::State::SLIDE) {
      player->_velocity.v.x =
          player->calcFriction(player->_velocity.v.x, SLIDE_DEACCELERATION, dt);
    }
  }
}

void SlideController::slide() {
  obj::State s = player->state;

  // No sliding while climbing
  if (s == obj::State::CLIMBING) {
    return;
  }

  // Slide
  if (s == obj::State::CROUCH) {
    // Check if on top of one-way platform. In that case we don't wanna slide
    // but deactivate the solid of the platform until we're below it
    LOG_INFO("On owp: %d", player->onOneWayPlatform);
    if (!player->onOneWayPlatform) {
      player->state = obj::State::SLIDE;
      slideTimer.reset();

      player->_animator.reset();
      player->_animator.setAnimation("slide");
      player->_velocity.v.x = player->direction == obj::Direction::LEFT
                                  ? -SLIDE_POWER
                                  : SLIDE_POWER;
    }
  }
}

void SlideController::onInputPressed(int button) {
  if (button == BUTTON::JUMP) {
    if (player->state == obj::State::CROUCH) {
      slide();
    }
  }
}
