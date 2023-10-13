#pragma once

#include "Vector.hpp"

struct Ray {
  float4 origin, direction;
  Ray() {}
  Ray(const float4& origin, const float4& direction)
      : origin(origin), direction(direction) {}
  inline float4 at(float t) const { return origin + direction * t; }
  inline float4 operator()(float t) const { return at(t); }
};