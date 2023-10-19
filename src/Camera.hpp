#pragma once

#include <omp.h>

#include "include/MathUtils.hpp"
#include "Interval.hpp"
#include "Hittable.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Image.hpp"
#include "Material.hpp"

struct Camera {
  static const float2 viewportCenter;
  static const float3 origin;

  int samplesPerPixel = 4;
  int maxDepth = 10;

  int width, height;
  mfloat widthF, heightF;
  mfloat aspectRatio;
  mfloat viewportHeight, viewportWidth;
  float2 viewportSize;

  Camera(int width, int height) : width(width), height(height) {
    widthF = width;
    heightF = height;
    aspectRatio = widthF / heightF;
    viewportHeight = 2;
    viewportWidth = viewportHeight * widthF / heightF;
    viewportSize = float2(viewportHeight, viewportWidth);
  }

  inline Image render(const HittableList &scene) {
    Image image(height, width, 3);

    int lines_rendered = 0;
    omp_lock_t lines_rendered_mutex;
    omp_init_lock(&lines_rendered_mutex);

#pragma omp parallel for num_threads(16)
    for (int x = 0; x < height; x++) {
      for (int y = 0; y < width; y++) {
        ColorF3 colorSum(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; s++) {
          auto samplePos = getRandomSamplePos(x, y);
          samplePos[0] /= heightF;
          samplePos[1] /= widthF;
          auto ray = rayToScreenPos(samplePos);
          colorSum += rayColor(ray, scene);
        }
        colorSum /= samplesPerPixel;
        image.setPixel(x, y, colorSum);
      }

      omp_set_lock(&lines_rendered_mutex);
      lines_rendered++;
      print("Lines rendered:", lines_rendered, "/", height);
      omp_unset_lock(&lines_rendered_mutex);
    }

    print("all lines rendered.");
    omp_destroy_lock(&lines_rendered_mutex);

    return image;
  }

  inline Ray rayToScreenPos(const float2 &screenPos) {
    // screenPos: (0, 1)^2
    // screenPosCentered: (-0.5, 0.5)^2
    float2 screenPosCentered = screenPos - viewportCenter;
    // a viewport plane at z = -1
    float2 worldPos = viewportSize * screenPosCentered;
    // dir coordinates: x: left, y: up, -z: depth
    float3 dir = float3(worldPos.y(), -worldPos.x(), -1);
    // emit a ray from the origin
    return Ray{origin, normalize(dir)};
  }

  inline float2 getRandomSamplePos(int x, int y) {
    // x in [0, height), y in [0, width)
    float2 screenPos(x, y);
    // delat.x, delta.y in [0, 1)
    float2 delta(RandFloat(), RandFloat());
    return screenPos + delta;
  }

  inline ColorF3 rayColor(const Ray &ray, const Hittable &scene,
                          int depth = 0) const {
    if (depth >= maxDepth)  // exceed the max depth
      return ColorF3(0, 0, 0);

    // ray trace
    auto result = scene.hit(ray, Interval(1e-3, INF));

    // hitted an object
    if (result.success) {
      auto hit = result.returnVal;

      auto matResult = hit.material->scatter(ray, hit);
      // not absorbed
      if (matResult.success) {
        auto scatteredRay = matResult.returnVal;
        // return ColorF3(0, 0, scatteredRay.ray.direction.pow() / 1.1f);
        // return saturate(scatteredRay.ray.direction);
        // return abs(scatteredRay.ray.direction);
        if (depth > 8 && typeid(*hit.material) == typeid(Dielectric)) {
          return ColorF3(0.1, 0.4, 0.9);
        }

        return scatteredRay.attenuation *
               rayColor(scatteredRay.ray, scene, depth + 1);
      }
      // absorbed
      return ColorF3(0, 0, 0);
    }

    // background color (sky color)
    float3 rayDirN = safeNormalize(ray.direction);
    mfloat blend = 0.5 * (rayDirN.y() + 1.0);
    ColorF3 color = lerp(ColorF3(1, 1, 1), ColorF3(0.5, 0.7, 1), blend);
    return color;
  }
};

const float2 Camera::viewportCenter = float2(0.5, 0.5);
const float3 Camera::origin = float3(0, 0, 0);
