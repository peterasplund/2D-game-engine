#include "settings.h"

GameSettings& gameSettings() {
  static GameSettings _settings;
  return _settings;
}
