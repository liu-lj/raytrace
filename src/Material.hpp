#pragma once

#include "include/Utils.hpp"
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
  virtual inline Result<ScatteredRay> scatter(const Ray& ray,
                                              const HitRecord& hit) const = 0;
};

struct Lambertian : public Material {
  ColorF3 albedo;
  Lambertian(ColorF3 albedo) : albedo(albedo) {}

  virtual inline Result<ScatteredRay> scatter(
      const Ray& ray, const HitRecord& hit) const override {
    auto scatterDir = hit.normal + RandomInUnitSphere();
    if (scatterDir.pow() < 1e-3) scatterDir = hit.normal;
    Ray scattered = Ray{hit.point, scatterDir.normalize()};
    return ScatteredRay{scattered, albedo};
  }
};

struct Metal : public Material {
  ColorF3 albedo;
  mfloat fuzz;
  Metal(ColorF3 albedo, mfloat fuzz)
      : albedo(albedo), fuzz(std::min(fuzz, mfloat(1))) {}

  virtual inline Result<ScatteredRay> scatter(
      const Ray& ray, const HitRecord& hit) const override {
    auto reflected = ReflectedVector(ray.direction, hit.normal);
    reflected += RandomInUnitSphere() * fuzz;
    if (reflected.dot(hit.normal) <= 0)
      // absorb the ray
      return false;
    Ray scattered = Ray{hit.point, reflected.normalize()};
    return ScatteredRay{scattered, albedo};
  }
};

// 电介质
struct Dielectric : public Material {
  mfloat refractiveIndex;
  Dielectric(mfloat refractiveIndex) : refractiveIndex(refractiveIndex) {}

  virtual inline Result<ScatteredRay> scatter(
      const Ray& ray, const HitRecord& hit) const override {
    auto relativeRefractiveIndex =
        hit.frontFace ? (1 / refractiveIndex) : refractiveIndex;
    auto rayIn = ray.direction;
    float3 normal = hit.normal * (hit.frontFace ? 1 : -1);

    // theta: angle between rayIn and normal, a.k.a. angle of incidence
    mfloat cosTheta = std::min(-1 * normal.dot(rayIn), mfloat(1));
    mfloat sinTheta = sqrt(1 - cosTheta * cosTheta);
    bool cannotRefract = relativeRefractiveIndex * sinTheta > 1;
    float3 direction;
    if (cannotRefract)
      direction = ReflectedVector(rayIn, normal);
    else
      direction = RefractedVector(rayIn, normal, relativeRefractiveIndex);
    Ray scattered = Ray{hit.point, normalize(direction)};
    return ScatteredRay{scattered, ColorF3(1, 1, 1)};
  }
};