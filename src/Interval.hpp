#pragma once

#include "include/MathUtils.hpp"

struct Interval {
  mfloat min, max;
  Interval() : min(-INF), max(INF) {}
  Interval(mfloat min, mfloat max) : min(min), max(max) {}
  mfloat length() const { return max - min; }
  bool contains(mfloat x) const { return x >= min && x <= max; }
  bool surroeds(mfloat x) const { return x > min && x < max; }
  mfloat clamp(mfloat x) const { return std::min(std::max(x, min), max); }

  static const Interval empty;
  static const Interval all;
};

const Interval Interval::empty = Interval(INF, -INF);
const Interval Interval::all = Interval();
