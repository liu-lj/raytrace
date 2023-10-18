#pragma once

#include "Utils.hpp"
#include "Ray.hpp"

struct HitRecord {
  float rayTime;
  float4 point;
  float3 normal;
  bool frontFace;
};

struct Hittable {
  // virtual ~Hittable() = default;

  virtual Result<HitRecord> hit(const Ray& ray, float tmin, float tmax) const = 0;
};