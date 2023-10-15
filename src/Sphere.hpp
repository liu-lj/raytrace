#pragma once

#include "Vector.hpp"
#include "Ray.hpp"
#include "Color.hpp"

struct Sphere {
  float radius;
  float4 center;
  ColorI4 color;

  inline Result<std::pair<float, float>> intersectTimes(const Ray& ray) const {
    float3 orig = ray.origin;
    float3 dir = ray.direction;
    float3 dis = center(0, 1, 2) - orig;
    float a = dir.dot(dir);
    float h = dir.dot(dis);
    float c = dis.pow() - radius * radius;
    float discriminant = h * h - a * c;

    using result = Result<std::pair<float, float>>;
    return discriminant >= 0
               ? result(std::make_pair((h - sqrt(discriminant)) / a,
                                       (h + sqrt(discriminant)) / a))
               : result();
  }
};