#pragma once

#include <vector>
#include <memory>

#include "Utils.hpp"
#include "Ray.hpp"

struct HitRecord {
  float rayTime;
  float4 point;
  float3 normal;
  bool frontFace;
};

struct Hittable {
  virtual ~Hittable() = default;

  virtual Result<HitRecord> hit(const Ray& ray, float tmin,
                                float tmax) const = 0;
};

struct HittableList : public Hittable {
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object) { add(object); }

  void clear() { objects.clear(); }

  void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

  Result<HitRecord> hit(const Ray& ray, float tmin, float tmax) const override {
    HitRecord record;
    auto timeOfClosestHit = tmax;

    for (const auto& object : objects) {
      auto result = object->hit(ray, tmin, timeOfClosestHit);
      if (result.success) {
        record = result.returnVal;
        timeOfClosestHit = record.rayTime;
      }
    }

    return record;
  }
};