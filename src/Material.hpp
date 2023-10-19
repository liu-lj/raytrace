#pragma once

#include "include/Result.hpp"
#include "Ray.hpp"
#include "Hittable.hpp"
#include "Color.hpp"

struct ScatteredRay {
  Ray ray;
  ColorF3 attenuation;
};

struct Material {
  ColorF3 color;
  Material() {}
  Material(ColorF3 color) : color(color) {}
  virtual inline Result<ScatteredRay> scatter(const Ray &ray,
                                              const HitRecord &hit) const = 0;
};

struct Lambertian : public Material {
  ColorF3 albedo;
  Lambertian(ColorF3 albedo) : albedo(albedo) {}

  virtual inline Result<ScatteredRay>
  scatter(const Ray &ray, const HitRecord &hit) const override {
    auto scatterDir = hit.normal + RandomInUnitSphere();
    if (scatterDir.pow() < 1e-3)
      scatterDir = hit.normal;
    Ray scattered = Ray{hit.point, scatterDir.normalized()};
    return ScatteredRay{scattered, albedo};
  }
};

struct Metal : public Material {
  ColorF3 albedo;
  Metal(ColorF3 albedo) : albedo(albedo) {}

  virtual inline Result<ScatteredRay>
  scatter(const Ray &ray, const HitRecord &hit) const override {
    auto reflected = ReflectedVector(ray.direction, hit.normal);
    Ray scattered = Ray{hit.point, reflected.normalized()};
    return ScatteredRay{scattered, albedo};
  }
};
