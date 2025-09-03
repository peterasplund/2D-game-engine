#include "climb.h"
#include <core/inputHandler.h>
#include "../../obj/player.h"

bool ClimbController::update(double dt) {
  Level *tilemap = EntityManager::Instance()->getTilemap();
  RectF rect = player->_collidable.rect;
  auto tilesWithin = player->_collidable.tileExistsAtF(rect);

  Tile *ladderAbove = player->tileAt<float, float>(
      {rect.x, rect.y - rect.h / 2, rect.w, rect.h / 2}, "Ladder");

  Tile *onLadder =
      player->tileAt<float, float>({rect.x, rect.y - 9, rect.w, rect.h - 9}, "Ladder");

  if (player->state == obj::State::CLIMBING) {
    player->_velocity.v.y = 0.0f;
    player->_renderable.textureFlip = SDL_FLIP_NONE;
    if (InputHandler::Instance()->isHeld(BUTTON::UP)) {
      player->_velocity.v.y = -0.1f;
    } else if (InputHandler::Instance()->isHeld(BUTTON::DOWN)) {
      player->_velocity.v.y = 0.1f;
    }

    player->_velocity.v.x = 0.0f;

    if (!ladderAbove) {
      player->_animator.setAnimation("crouch");
    } else {
      player->_animator.setAnimation("climb");
      player->_animator.stop();
    }

    if (player->_velocity.v.y < -0.001f) {
      if (!player->_animator.isPlaying()) {
        player->_animator.start();
      }
    } else if (player->_velocity.v.y > 0.001f) {
      if (!player->_animator.isPlaying()) {
        player->_animator.start();
      }
      // @TODO: reverse animation
    }

    if (onLadder == nullptr/* && !hasTileAbove*/) {
      player->state = obj::State::IDLE;
    }

    return true;
  }


  // Climb onto ladder
  if (tilesWithin.size() > 0) {
    for (auto tile : tilesWithin) {
      int tileCenter = tile.rect.x + tile.rect.w / 2;
      int playerCenter = rect.x + rect.w / 2;
      if (playerCenter > tileCenter - LADDER_X_DEADZONE &&
          playerCenter < tileCenter + LADDER_X_DEADZONE) {
        if (InputHandler::Instance()->isHeld(BUTTON::UP)) {
          Tileset *tileset = &tilemap->world->tilesetDefs[tile.tilesetId];

          if (tileset->tileHasTag(tile.tile.getTileId(), "Ladder")) {
            if (tile.rect.top() <= rect.bottom() - 30) {
              player->_position.x = tile.rect.x - 23;
              player->state = obj::State::CLIMBING;
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}
