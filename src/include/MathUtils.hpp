#pragma once

#include <random>
#include <limits>
#include <algorithm>

constexpr const mfloat PI = 3.1415927f;
constexpr const mfloat INF = std::numeric_limits<mfloat>().infinity();

template <typename T, typename T2>
  requires std::is_arithmetic_v<T2>
inline decltype(auto) lerp(T a, T b, T2 t) {
  return a * (1 - t) + b * t;
}

using std::clamp;
// template <typename T>
// inline T clamp(T x, T min, T max) {
//   return std::min(std::max(x, min), max);
// }

template <typename T>
inline T saturate(T x) { return clamp(x, 0, 1); }

inline mfloat Deg2Rad(mfloat degrees) { return degrees * PI / 180; }

// rand float in [0, 1)
inline mfloat RandFloat() {
  static std::uniform_real_distribution<mfloat> distribution(0, 1);
  static std::mt19937 generator;
  return distribution(generator);
}

// rand float in [min, max)
inline mfloat RandFloat(mfloat min, mfloat max) {
  std::uniform_real_distribution<mfloat> distribution(min, max);
  static std::mt19937 generator;
  return distribution(generator);
}