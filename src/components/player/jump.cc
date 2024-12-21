#include "jump.h"
#include "../../engine/inputHandler.h"
#include "../../obj/player.h"

void JumpController::update(double dt) {
  // Jump buffer
  if (jumpBufferTimer.elapsed() < JUMP_BUFFER_WINDOW &&
      player->_gravity.onFloor &&
      InputHandler::Instance()->isHeld(BUTTON::JUMP)) {
    if (player->state == obj::State::IDLE || player->state == obj::State::RUN) {
      performJump();
    }
  }
}

void JumpController::performJump() {
  jumpBuffered = false;
  player->state = obj::State::JUMP;
  player->_velocity.v.y = -JUMP_POWER;
  jumpHold = true;
}

void JumpController::jump() {
  obj::State s = player->state;
  if (s != obj::State::ATTACK && s != obj::State::CROUCH) {
    if (player->_gravity.onFloor || s == obj::State::SLIDE || s == obj::State::CLIMBING) {
      performJump();
    }
    if (!player->_gravity.onFloor) {
      jumpBufferTimer.reset();
      jumpBuffered = true;
    }
  }
}

void JumpController::onInputPressed(int button) {
  if (button == BUTTON::JUMP) {
    if (player->state != obj::State::CROUCH && !player->hasTileAbove) {
      jump();
    }
  }
}

void JumpController::onInputReleased(int button) {
  if (button == BUTTON::JUMP) {
    jumpHold = false;
  }
}
