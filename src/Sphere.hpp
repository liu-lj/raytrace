#pragma once

#include <memory>

#include "include/Result.hpp"
#include "Ray.hpp"
#include "Color.hpp"
#include "Hittable.hpp"
#include "Material.hpp"

struct Sphere : public Hittable {
  mfloat radius;
  float3 center;
  std::shared_ptr<Material> material;

  Sphere() {}
  Sphere(mfloat radius, float3 center, std::shared_ptr<Material> material)
      : radius(radius), center(center), material(material) {}

  inline Result<HitRecord> hit(const Ray &ray,
                               Interval rayTime) const override {
    float3 orig = ray.origin;
    float3 dir = ray.direction;
    float3 dis = center - orig;
    mfloat a = dir.dot(dir);
    mfloat h = dir.dot(dis);
    mfloat c = dis.pow() - radius * radius;
    mfloat discriminant = h * h - a * c;
    if (discriminant < 0) return false;

    mfloat sqrtd = sqrt(discriminant);
    mfloat t1 = (h - sqrtd) / a;
    mfloat time = t1;
    if (t1 < rayTime.min || t1 > rayTime.max) {
      // t1 not in range, use t2
      mfloat t2 = (h + sqrtd) / a;
      time = t2;
      if (t2 < rayTime.min || t2 > rayTime.max)  // t2 not in range
        return false;
    }

    float3 point = ray(time);
    float3 normal = (point - center) / radius;
    // normal 和 dir 异向，说明射线从球外部射入，为正面
    bool isFrontFace = normal.dot(dir) < 0;
    return HitRecord{
        time,         // ray time
        point,        // hit point
        normal,       // normal
        isFrontFace,  // front face
        material      // material
    };
  }
};