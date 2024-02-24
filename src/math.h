#pragma once

#include "SDL.h"

template<class T>
struct v2
{
  static_assert(std::is_arithmetic<T>::value, "olc::v_2d<type> must be numeric");

  T x = 0;
  T y = 0;

  // Constructor
  inline constexpr v2() = default;
  inline constexpr v2(T _x, T _y) : x(_x), y(_y) {}

  inline auto magnitude() {
    return sqrt((x * x) + (y * y));
  }

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
      return sqrt(dx*dx + dy*dy);
  }

  // Allow casting from any v2 type
  template<class F>
    inline constexpr operator v2<F>() const {
      return { static_cast<F>(this->x), static_cast<F>(this->y) };
    }
};

// Negation
template<class T>
inline constexpr auto operator - (const v2<T>& lhs) {
  return v2(-lhs.x, -lhs.y);
}

// Multiplication
template<class TL, class TR>
inline constexpr auto operator * (const TL& lhs, const v2<TR>& rhs) {
  return v2(lhs * rhs.x, lhs * rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator * (const v2<TL>& lhs, const TR& rhs) {
  return v2(lhs.x * rhs, lhs.y * rhs);
}

template<class TL, class TR>
inline constexpr auto operator * (const v2<TL>& lhs, const v2<TR>& rhs) {
  return v2(lhs.x * rhs.x, lhs.y * rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator *= (const v2<TL>& lhs, const TR& rhs) {
  lhs = lhs * rhs;
  return lhs;
}

// Division
template<class TL, class TR>
inline constexpr auto operator / (const TL& lhs, const v2<TR>& rhs) {
  return v2(lhs / rhs.x, lhs / rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator / (const v2<TL>& lhs, const TR& rhs) {
  return v2(lhs.x / rhs, lhs.y / rhs);
}

template<class TL, class TR>
inline constexpr auto operator / (const v2<TL>& lhs, const v2<TR>& rhs) {
  return v2(lhs.x / rhs.x, lhs.y / rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator /= (const v2<TL>& lhs, const TR& rhs) {
  lhs = lhs / rhs;
  return lhs;
}

// Addition
template<class TL, class TR>
inline constexpr auto operator + (const TL& lhs, const v2<TR>& rhs) {
  return v2(lhs + rhs.x, lhs + rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator + (const v2<TL>& lhs, const TR& rhs) {
  return v2(lhs.x + rhs, lhs.y + rhs);
}

template<class TL, class TR>
inline constexpr auto operator + (const v2<TL>& lhs, const v2<TR>& rhs) {
  return v2(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator += (const v2<TL>& lhs, const TR& rhs) {
  lhs = lhs + rhs;
  return lhs;
}

template<class TL, class TR>
inline constexpr auto operator += (const v2<TL>& lhs, const v2<TR>& rhs) {
  lhs = lhs + rhs;
  return lhs;
}

// Subtraction
template<class TL, class TR>
inline constexpr auto operator - (const TL& lhs, const v2<TR>& rhs) {
  return v2(lhs - rhs.x, lhs - rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator - (const v2<TL>& lhs, const TR& rhs) {
  return v2(lhs.x - rhs, lhs.y - rhs);
}

template<class TL, class TR>
inline constexpr auto operator - (const v2<TL>& lhs, const v2<TR>& rhs) {
  return v2(lhs.x - rhs.x, lhs.y - rhs.y);
}

template<class TL, class TR>
inline constexpr auto operator -= (const v2<TL>& lhs, const TR& rhs) {
  lhs = lhs - rhs;
  return lhs;
}


typedef v2<int> v2i;
typedef v2<float> v2f;

struct Rect {
  int x;
  int y;
  int w;
  int h;

  int top() { return y; }
  int bottom() { return y + h; }
  int left() { return x; }
  int right() { return x + w; }

  v2i pos() {
    return { x, y };
  }

  v2i size() {
    return { w, h };
  }

  void setPos(v2i v) {
    x = v.x;
    y = v.y;
  }

  void setSize(v2i v) {
    w = v.x;
    h = v.y;
  }

  bool hasIntersection(Rect* other) {
    if (right() > other->x && x < other->right()) {
      if (bottom() > other->y && y < other->bottom()) {
        return true;
      }
    }

    return false;
  }

  static Rect from_sdl_rect(SDL_Rect r) {
    return Rect {
      r.x,
      r.y,
      r.w,
      r.h,
    };
  }

  SDL_Rect to_sdl_rect() {
    return SDL_Rect {
      this->x,
      this->y,
      this->w,
      this->h,
    };
  }

  // @TODO: place this behind compiler flag
  void debug() {
    printf("(x: %d\ty: %d\tw: %d\th: %d)\n", x, y, w, h);
  }
};

struct RectF {
  float x;
  float y;
  float w;
  float h;

  int top() { return y; }
  int bottom() { return y + h; }
  int left() { return x; }
  int right() { return x + w; }

  v2f pos() {
    return { x, y };
  }

  v2f size() {
    return { w, h };
  }

  void setPos(v2f v) {
    x = v.x;
    y = v.y;
  }

  void setSize(v2f v) {
    w = v.x;
    h = v.y;
  }

  bool hasIntersection(RectF* other) {
    if (right() > other->x && x < other->right()) {
      if (bottom() > other->y && y < other->bottom()) {
        return true;
      }
    }

    return false;
  }

  static Rect from_sdl_rect(SDL_Rect r) {
    return Rect {
      r.x,
      r.y,
      r.w,
      r.h,
    };
  }

  SDL_Rect to_sdl_rect() {
    return SDL_Rect {
      (int)round(this->x),
      (int)round(this->y),
      (int)round(this->w),
      (int)round(this->h),
    };
  }

  // @TODO: place this behind compiler flag
  void debug() {
    printf("(x: %f\ty: %f\tw: %f\th: %f)\n", x, y, w, h);
  }
};

/*
RectF group(RectF r1, RectF r2) {
  float x = std::min(r1.x, r2.x);
  float y = std::min(r1.y, r2.y);

  float max_right = std::max(r1.right(), r2.right());
  float max_bottom = std::max(r1.bottom(), r2.bottom());

  return {
    x,
    y,
    max_right - x,
    max_bottom - y,
  };
}
*/

Rect group_rects(RectF r1, RectF r2);
