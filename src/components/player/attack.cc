#include "attack.h"
#include "../../debugPrinter.h"
#include "../../engine/inputHandler.h"
#include "../../obj/damageNumbers.h"
#include "../../obj/player.h"
#include "../../obj/player.h"

void AttackController::update(double dt) {
  obj::State s = player->state;

  swordHitBox = {
      player->_position.x +
          (player->direction == Direction::LEFT ? -0 : 36),
      player->_position.y + 5.0f,
      20.0f,
      40.0f,
  };

  // End attack
  if (attackTimer.elapsed() > attackDelay && s == obj::State::ATTACK) {
    player->state = obj::State::IDLE;
    player->_animator.reset();
    player->_animator.setAnimation("idle");
  }

  // Check hitbox
  auto entitiesTouchingSword = player->_collidable.objectExistsAt(swordHitBox);
  for (auto entity : entitiesTouchingSword) {
    if (entity->getType() == GAME_OBJECT::ENEMY && s == obj::State::ATTACK) {
      int value = ((float)(rand()) / (float)RAND_MAX) * 1000;
      if (!entity->hurt && !entity->invincible) {
        DamageNumbersSystem::Instance()->addNumber(
            value, {(int)(swordHitBox.left() + swordHitBox.w / 2 + 8),
                    (int)swordHitBox.top()});

        entity->damage(value);

        if (player->direction == Direction::RIGHT) {
          entity->_velocity.v.x = 0.2f;
        } else {
          entity->_velocity.v.x = -0.2f;
        }
      }
    }
  }
}

void AttackController::attack() {
  if (attackTimer.elapsed() > attackDelay || firstAttack) {
    firstAttack = false;
    player->state = obj::State::ATTACK;
    player->_animator.reset();
    player->_animator.setAnimation("attack");
    attackTimer.reset();
  }
}

void AttackController::onInputPressed(int button) {
  if (button == BUTTON::ATTACK && !player->hasTileAbove) {
    attack();
  }
}

void AttackController::draw(Renderer *renderer) {
  if (player->state == obj::State::ATTACK) {
    Rect hitbox = {(int)swordHitBox.x, (int)swordHitBox.y, (int)swordHitBox.w,
                   (int)swordHitBox.h};
    DebugPrinter::Instance()->addDebugRect(&hitbox, 0, 255, 0);
  }
}
