#pragma once

#include "Utils.hpp"

struct Interval {
  float min, max;
  Interval() : min(-INF), max(INF) {}
  Interval(float min, float max) : min(min), max(max) {}
  inline float length() const { return max - min; }
  inline bool contains(float x) const { return x >= min && x <= max; }
  inline bool surroeds(float x) const { return x > min && x < max; }
  inline float clamp(float x) const { return std::min(std::max(x, min), max); }

  static const Interval empty;
  static const Interval all;
};

const Interval Interval::empty = Interval(INF, -INF);
const Interval Interval::all = Interval();
