#pragma once

#include "Vector.hpp"
#include "Ray.hpp"
#include "Color.hpp"
#include "Hittable.hpp"

struct Sphere : public Hittable {
  float radius;
  float4 center;
  ColorI4 color;

  inline Result<HitRecord> hit(const Ray& ray, float tmin, float tmax) const {
    float3 orig = ray.origin;
    float3 dir = ray.direction;
    float3 dis = center(0, 1, 2) - orig;
    float a = dir.dot(dir);
    float h = dir.dot(dis);
    float c = dis.pow() - radius * radius;
    float discriminant = h * h - a * c;
    if (discriminant < 0) return Result<HitRecord>();

    float sqrtd = sqrt(discriminant);
    float t1 = (h - sqrtd) / a;
    float time = t1;
    if (t1 < tmin || t1 > tmax) {
      // t1 not in range, use t2
      float t2 = (h + sqrtd) / a;
      time = t2;
      if (t2 < tmin || t2 > tmax)  // t2 not in range
        return Result<HitRecord>();
    }

    float4 point = ray(time);
    float3 normal = (point - center) / radius;
    // normal 和 dir 异向，说明射线从球外部射入，为正面
    bool isFrontFace = normal.dot(dir) < 0; 
    return Result<HitRecord>(HitRecord{
        time,   // ray time
        point,  // hit point
        normal,  // normal
        isFrontFace  // front face
    });
  }
};