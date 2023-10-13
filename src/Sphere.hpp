#pragma once

#include "Vector.hpp"
#include "Ray.hpp"
#include "Color.hpp"

struct Sphere {
  float radius;
  float4 center;
  ColorI4 color;
  Sphere() {}
  Sphere(float4 center, float radius, ColorI4 color) : center(center), radius(radius), color(color) {}
  bool intersect(const Ray& ray, float& t0, float& t1) const {
   float3 orig = ray.origin;
   float3 dir = ray.direction;
   float3 dis = orig - center(0, 1, 2);
   float a = dir.dot(dir);
   float b = 2 * dir.dot(dis);
   float c = dis.dot(dis) - radius * radius;
   float discriminant = b * b - 4 * a * c;
   if (discriminant > 0) {
     t0 = (-b - sqrt(discriminant)) / 2 / a;
     t1 = (-b + sqrt(discriminant)) / 2 / a;
     return true;
   }
   return false;
  }
  bool intersect(const Ray& ray) const {
   float3 orig = ray.origin;
   float3 dir = ray.direction;
   float3 dis = orig - center;
   float a = dir.dot(dir);
   float b = 2 * dir.dot(dis);
   float c = dis.dot(dis) - radius * radius;
   float discriminant = b * b - 4 * a * c;
   return discriminant > 0;
  }
};