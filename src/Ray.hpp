#pragma once

#include "include/Vector.hpp"

struct Ray {
  float3 origin, direction;

  inline float3 at(mfloat t) const { return origin + direction * t; }
  inline float3 operator()(mfloat t) const { return at(t); }
};