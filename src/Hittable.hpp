#pragma once

#include <vector>
#include <memory>

#include "Interval.hpp"
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

  virtual inline Result<HitRecord> hit(const Ray &ray,
                                       Interval rayTime) const = 0;
};

struct HittableList : public Hittable {
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object) { add(object); }

  inline void clear() { objects.clear(); }

  inline void add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
  }

  inline Result<HitRecord> hit(const Ray &ray,
                               Interval rayTime) const override {
    Result<HitRecord> record;

    for (const auto &object : objects) {
      auto result = object->hit(ray, rayTime);
      if (result.success) {
        record = result;
        rayTime.max = record.returnVal.rayTime;
      }
    }

    return record;
  }
};