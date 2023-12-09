#pragma once

#include "include/Vector.hpp"

struct Ray {
  float3 origin, direction;

  float3 at(mfloat t) const { return origin + direction * t; }
  float3 operator()(mfloat t) const { return at(t); }
};