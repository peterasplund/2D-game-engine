#pragma once

#include <vector>
#include "math.h"
#include "entityManager.h"
#include "abstractGameobject.h"
#include "imgui_layer.h"

struct DebugRect {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  SDL_Rect rect;
};

class DebugPrinter {
private:
  bool* debugRectangles;
  std::vector<DebugRect> debug_rects;

  DebugPrinter() {
    debugRectangles = ImguiLayer::Instance()->getDebugRectanglesBool();
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
