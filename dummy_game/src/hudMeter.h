#pragma once

#include <core/assetManager.h>
#include <core/math.h>
#include <core/timer.h>

// @TODO: implement flash latest change in another color.
class HudMeter {
public:
  HudMeter(Rect rect, SDL_Color color) {
    _rect = rect;
    _color = color;
    _barTexture =
        AssetManager::Instance()->getTexture("assets/sprites/bar.png");
  }
  void render(SDL_Renderer *renderer, int value, int maxValue) {
    float fraction = value == 0 ? 0.0f : (double)value / (double)maxValue;

    if (_lastValue != -1 && value != _lastValue) {
      _timer.reset();
      _animateFromFraction = _lastValue == 0 ? 0.0f : (double)_lastValue / (double)maxValue;
    }

    if (_timer.elapsed() < lerpTiming && _timer.elapsed() != 0.0f && _animateFromFraction != -1) {
      float time = (float)_timer.elapsed() / (float)lerpTiming;
      fraction = lerp(
        _animateFromFraction,
        fraction,
        easing(time)
      );
    }


    int valueWidth = (_rect.w - _rect.x) * fraction;

    SDL_Rect bgRectSrc = {0, 0, _rect.w, _rect.h};
    SDL_Rect bgRectDist = {_rect.x, _rect.y, _rect.w, _rect.h};
    SDL_RenderCopy(renderer, _barTexture, &bgRectSrc, &bgRectDist);

    if (valueWidth > 0) {
      SDL_Rect filled = {_rect.x + 2, _rect.y + 2, valueWidth, _rect.h - 4};
      SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
      SDL_RenderFillRect(renderer, &filled);
    }

    _lastValue = value; 
  }

protected:
  SDL_Texture *_barTexture;
  SDL_Color _color;
  Rect _rect;
  int _lastValue = -1;
  float _animateFromFraction = -1;

  int lerpTiming = 100;

  Timer _timer;
};
