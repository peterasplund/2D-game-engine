#include "entityManager.h"
#include "abstractGameobject.h"
#include "engine/logger.h"
#include "obj/bat.h"
#include "obj/npc.h"
#include "obj/player.h"

// Use some configuration place to specify all game objects. Maybe even glob the
// object directory (bad idea?)
AbstractGameObject *EntityManager::instantiateGameObject(GAME_OBJECT obj) {
  AbstractGameObject *o = nullptr;
  switch (obj) {
  case GAME_OBJECT::PLAYER:
    o = new obj::Player();
    break;
  case GAME_OBJECT::NPC:
    o = new obj::Npc();
    break;
  }

  return o;
}

EntityManager::EntityManager() {
  _entities = std::vector<AbstractGameObject *>();

  this->_camera = camera();
}

EntityManager *EntityManager::Instance() {
  static EntityManager *_instance = nullptr;
  if (_instance == nullptr) {
    _instance = new EntityManager();
  }

  return _instance;
}

void EntityManager::addEntity(AbstractGameObject *x) {
  _entities.push_back(std::move(x));

  // Add easy access to some global objects
  if (x->getTag() == OBJECT_TAG::PLAYER) {
    _player = x;
  }
}

void EntityManager::setTileMap(Level *x) { _tilemap = x; }

const std::vector<AbstractGameObject *> &EntityManager::getEntities() {
  return _entities;
}

// This is a bit naive without a good data structure but we'll be fine.
AbstractGameObject *EntityManager::getEntityByTag(OBJECT_TAG tag) {
  for (const auto &obj : _entities) {
    if (obj->getTag() == tag) {
      return obj;
    }
  }

  LOG_WARN(
      "ERROR: Tried getting an entity by a tag that doesn't exist. Tag: %d",
      (int)tag);
  return nullptr;
}

Level *EntityManager::getTilemap() { return _tilemap; }

void EntityManager::release() {
  auto i = Instance()->_entities;
  std::vector<AbstractGameObject *>::iterator it;
  for (it = Instance()->_entities.begin(); it != Instance()->_entities.end();) {
    delete (*it);
    it = Instance()->_entities.erase(it);
  }
}

void EntityManager::update() {
  std::vector<AbstractGameObject *>::iterator it;
  for (it = _entities.begin(); it != _entities.end();) {
    if ((*it)->dead) {
      delete (*it);
      it = _entities.erase(it);
    } else {
      ++it;
    }
  }
}

void EntityManager::instantiateLevelEntitites(World *world, Level *level) {
  AbstractGameObject *bat = new obj::Bat();
  bat->init();
  addEntity(bat);

  std::map<std::string, GAME_OBJECT> gameObjects = {
      {"Player", GAME_OBJECT::PLAYER},
  };

  for (auto layer : level->layers) {
    for (auto e : layer.entities) {
      auto entityDef = world->entityDefs[e.uid];

      if (entityDef.identifier == "NPC") {
        auto npc = instantiateGameObject(GAME_OBJECT::NPC);

        npc->_position = {(float)e.position.x, (float)e.position.y};

        npc->init();

        std::string name;
        std::string dialogue;
        for (auto field : e.fieldValues) {
          LOG_TRACE("Parsed field: %s", field.identifier.c_str());
          if (field.identifier == "name") {
            name = field.value;
          }
          if (field.identifier == "dialogue") {
            dialogue = field.value;
          }
        }

        ((obj::Npc *)npc)->setProperties(name, dialogue);

        addEntity(npc);
      } else {
        // Tmp solution
        if (entityDef.identifier == "Player" && _player != nullptr) {
          continue;
        }

        auto it = gameObjects.find(entityDef.identifier);

        if (it == gameObjects.end()) {
          continue;
        }

        auto object = instantiateGameObject(it->second);
        if (object != nullptr) {
          object->_position = {(float)e.position.x, (float)e.position.y};
          object->init();

          addEntity(object);
        }
      }
    }
  }

  // Sort by zIndex to always render in the correct order.
  std::sort(_entities.begin(), _entities.end(), [](const AbstractGameObject* a, const AbstractGameObject* b) {
      return a->_zIndex < b->_zIndex;
  });
}
