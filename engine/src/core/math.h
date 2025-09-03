#pragma once

#include <SDL2/SDL_rect.h>
#include "logger.h"

int min(int a, int b);
int max(int a, int b);

template <class T> struct v2 {
  static_assert(std::is_arithmetic<T>::value, "v2<type> must be numeric");

  T x = 0;
  T y = 0;

  // Constructor
  inline constexpr v2() = default;
  inline constexpr v2(T _x, T _y) : x(_x), y(_y) {}

  // Copy constructor
  inline constexpr v2(const v2 &v) = default;

  // Assignment operator
  inline constexpr v2 &operator=(const v2 &v) = default;

  inline auto magnitude() { return sqrt((x * x) + (y * y)); }

  void normalize() {
    if ((x != 0) || (y != 0)) {
      float factor = 1 / sqrt((x * x) + (y * y));
      x = factor * x;
      y = factor * y;
    }
  }

  float distanceTo(v2<T> vec) {
    float dx = x - vec.x;
    float dy = y - vec.y;
    return sqrt(dx * dx + dy * dy);
  }

  // Compare if this vector is numerically equal to another
  inline constexpr bool operator==(const v2 &rhs) const {
    return (this->x == rhs.x && this->y == rhs.y);
  }

  // Compare if this vector is not numerically equal to another
  inline constexpr bool operator!=(const v2 &rhs) const {
    return (this->x != rhs.x || this->y != rhs.y);
  }

  // Allow casting from any v2 type
  template <class F> inline constexpr operator v2<F>() const {
    return {static_cast<F>(this->x), static_cast<F>(this->y)};
  }
};

// Negation
template <class T> inline constexpr auto operator-(const v2<T> &lhs) {
  return v2(-lhs.x, -lhs.y);
}

// Multiplication
template <class TL, class TR>
inline constexpr auto operator*(const TL &lhs, const v2<TR> &rhs) {
  return v2(lhs * rhs.x, lhs * rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator*(const v2<TL> &lhs, const TR &rhs) {
  return v2(lhs.x * rhs, lhs.y * rhs);
}

template <class TL, class TR>
inline constexpr auto operator*(const v2<TL> &lhs, const v2<TR> &rhs) {
  return v2(lhs.x * rhs.x, lhs.y * rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator*=(const v2<TL> &lhs, const TR &rhs) {
  lhs = lhs * rhs;
  return lhs;
}

// Division
template <class TL, class TR>
inline constexpr auto operator/(const TL &lhs, const v2<TR> &rhs) {
  return v2(lhs / rhs.x, lhs / rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator/(const v2<TL> &lhs, const TR &rhs) {
  return v2(lhs.x / rhs, lhs.y / rhs);
}

template <class TL, class TR>
inline constexpr auto operator/(const v2<TL> &lhs, const v2<TR> &rhs) {
  return v2(lhs.x / rhs.x, lhs.y / rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator/=(const v2<TL> &lhs, const TR &rhs) {
  lhs = lhs / rhs;
  return lhs;
}

// Addition
template <class TL, class TR>
inline constexpr auto operator+(const TL &lhs, const v2<TR> &rhs) {
  return v2(lhs + rhs.x, lhs + rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator+(const v2<TL> &lhs, const TR &rhs) {
  return v2(lhs.x + rhs, lhs.y + rhs);
}

template <class TL, class TR>
inline constexpr auto operator+(const v2<TL> &lhs, const v2<TR> &rhs) {
  return v2(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator+=(v2<TL> &lhs, const TR &rhs) {
  lhs = lhs + rhs;
  return lhs;
}

template <class TL, class TR>
inline constexpr auto operator+=(v2<TL> &lhs, const v2<TR> &rhs) {
  lhs = lhs + rhs;
  return lhs;
}

// Subtraction
template <class TL, class TR>
inline constexpr auto operator-(const TL &lhs, const v2<TR> &rhs) {
  return v2(lhs - rhs.x, lhs - rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator-(const v2<TL> &lhs, const TR &rhs) {
  return v2(lhs.x - rhs, lhs.y - rhs);
}

template <class TL, class TR>
inline constexpr auto operator-(const v2<TL> &lhs, const v2<TR> &rhs) {
  return v2(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <class TL, class TR>
inline constexpr auto operator-=(const v2<TL> &lhs, const TR &rhs) {
  lhs = lhs - rhs;
  return lhs;
}

template <class T, class J> struct Rectangle {
  T x;
  T y;
  J w;
  J h;

  T top() { return y; }
  T bottom() { return y + h; }
  T left() { return x; }
  T right() { return x + w; }

  v2<T> pos() { return {x, y}; }

  v2<J> size() { return {w, h}; }

  void setPos(v2<T> v) {
    x = v.x;
    y = v.y;
  }

  void setSize(v2<J> v) {
    w = v.x;
    h = v.y;
  }

  bool hasIntersection(Rectangle<int, int> *other) {
    if (right() > other->x && x < other->right()) {
      if (bottom() > other->y && y < other->bottom()) {
        return true;
      }
    }

    return false;
  }

  bool hasIntersection(Rectangle<float, float> *other) {
    if (right() > other->x && x < other->right()) {
      if (bottom() > other->y && y < other->bottom()) {
        return true;
      }
    }

    return false;
  }

  static Rectangle from_sdl_rect(SDL_Rect r) {
    return Rectangle{
        r.x,
        r.y,
        r.w,
        r.h,
    };
  }

  SDL_Rect to_sdl_rect() const {
    return SDL_Rect{
        (int)round(this->x),
        (int)round(this->y),
        (int)round(this->w),
        (int)round(this->h),
    };
  }

  // @TODO: place this behind compiler flag
  void debug() {
    LOG_DEBUG("(x: %f\ty: %f\tw: %f\th: %f)\n", x, y, w, h);
  }

  void debugInt() {
    LOG_DEBUG("(x: %d\ty: %d\tw: %d\th: %d)\n", x, y, w, h);
  }
};

typedef v2<int> v2i;
typedef v2<float> v2f;
typedef Rectangle<float, float> RectF;
typedef Rectangle<int, int> Rect;

Rectangle<int, int> group_rects(Rectangle<float, float> r1,
                                Rectangle<float, float> r2);

float lerp(float v0, float v1, float t);
float easing(float t);
