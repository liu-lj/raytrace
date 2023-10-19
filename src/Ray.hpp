#pragma once

#include "include/Vector.hpp"

struct Ray {
  float3 origin, direction;

  inline float3 at(float t) const { return origin + direction * t; }
  inline float3 operator()(float t) const { return at(t); }
};