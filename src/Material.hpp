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

  inline Result<ScatteredRay> scatter(const Ray& ray,
                                      const HitRecord& hit) const override {
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

  inline Result<ScatteredRay> scatter(const Ray& ray,
                                      const HitRecord& hit) const override {
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

  inline Result<ScatteredRay> scatter(const Ray& ray,
                                      const HitRecord& hit) const override {
    // relative refractive index
    auto rri = hit.frontFace ? (1 / refractiveIndex) : refractiveIndex;
    auto rayIn = ray.direction;
    float3 normal = hit.normal * (hit.frontFace ? 1 : -1);

    // theta: angle between rayIn and normal, a.k.a. angle of incidence
    mfloat cosTheta = std::min(-1 * normal.dot(rayIn), mfloat(1));
    mfloat sinTheta = sqrt(1 - cosTheta * cosTheta);
    bool cannotRefract = rri * sinTheta > 1;
    float3 direction;
    if (cannotRefract || reflectance(cosTheta, rri) > RandFloat())
      direction = ReflectedVector(rayIn, normal);
    else
      direction = RefractedVector(rayIn, normal, rri);
    Ray scattered = Ray{hit.point, normalize(direction)};
    return ScatteredRay{scattered, ColorF3(1, 1, 1)};
  }

  // Use Schlick's approximation for reflectance
  inline static mfloat reflectance(mfloat cos, mfloat relativeRefractiveIndex) {
    auto r0 = (1 - relativeRefractiveIndex) / (1 + relativeRefractiveIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cos), 5);
  }
};