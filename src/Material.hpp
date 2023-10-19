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
  float fuzz;
  Metal(ColorF3 albedo, float fuzz)
      : albedo(albedo), fuzz(std::min(fuzz, 1.0f)) {}

  virtual inline Result<ScatteredRay>
  scatter(const Ray &ray, const HitRecord &hit) const override {
    auto reflected = ReflectedVector(ray.direction, hit.normal);
    reflected += RandomInUnitSphere() * fuzz;
    if (reflected.dot(hit.normal) <= 0)
      // absorb the ray
      return Result<ScatteredRay>();
    Ray scattered = Ray{hit.point, reflected.normalized()};
    return ScatteredRay{scattered, albedo};
  }
};

// 电介质
struct Dielectric : public Material {
  float refractiveIndex;
  Dielectric(float refractiveIndex) : refractiveIndex(refractiveIndex) {}

  virtual inline Result<ScatteredRay>
  scatter(const Ray &ray, const HitRecord &hit) const override {
    auto relativeRefractiveIndex =
        hit.frontFace ? (1 / refractiveIndex) : refractiveIndex;
    auto refracted = RefractedVector(normalize(ray.direction), hit.normal,
                                     relativeRefractiveIndex);
    Ray scattered = Ray{hit.point, normalize(refracted)};
    return ScatteredRay{scattered, ColorF3(1, 1, 1)};
  }
};