#pragma once

#include "Ray.hpp"
#include "Hittable.hpp"
#include "Color.hpp"

struct Material
{
  ColorF3 color;
  Material() {}
  Material(ColorF3 color) : color(color) {}
  // virtual bool scatter(
  //       const Ray& ray, const HitRecord& hit, ColorI3& attenuation, Ray& scattered) const = 0;
};

struct Lambertian : public Material
{
  using Material::Material;
};

struct Metal : public Material
{
  using Material::Material;
};
