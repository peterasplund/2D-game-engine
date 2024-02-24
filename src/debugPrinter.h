#pragma once

#include <vector>
#include "math.h"
#include "entityManager.h"
#include "abstractGameobject.h"

struct DebugRect {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  SDL_Rect rect;
};

class DebugPrinter {
private:
  std::vector<DebugRect> debug_rects;

  DebugPrinter() {
  }

public:
  static DebugPrinter* Instance() {
    static DebugPrinter* _instance = nullptr;
    if (_instance == nullptr) {
      _instance = new DebugPrinter();
    }

    return _instance;
  }

  static void release() {
  }

  void addDebugRect(Rect* rect, Uint8 r, Uint8 g, Uint8 b);
  void draw(SDL_Renderer* renderer);
  void drawHitboxes(SDL_Renderer* renderer, Rect camera);
};
