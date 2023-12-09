#pragma once

#include <vector>
#include <memory>

#include "include/Utils.hpp"
#include "Ray.hpp"
#include "Interval.hpp"
// don't import "Material.hpp" here

// avoid circular dependency
struct Material;

struct HitRecord {
  mfloat rayTime;
  float3 point;
  float3 normal;
  bool frontFace;
  std::shared_ptr<Material> material;
};

struct Hittable {
  // virtual ~Hittable() = default;

  virtual Result<HitRecord> hit(const Ray &ray, Interval rayTime) const = 0;
};

struct HittableList : public Hittable {
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object) { add(object); }

  void clear() { objects.clear(); }

  void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

  Result<HitRecord> hit(const Ray &ray, Interval rayTime) const override {
    Result<HitRecord> record;

    for (const auto &object : objects) {
      auto result = object->hit(ray, rayTime);
      if (result.success) {
        record = result;
        rayTime.max = record.ret.rayTime;
      }
    }

    return record;
  }
};