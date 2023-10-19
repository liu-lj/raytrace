#pragma once

#include <omp.h>

#include "include/MathUtils.hpp"
#include "Interval.hpp"
#include "Hittable.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Image.hpp"
#include "Material.hpp"

struct CameraTransform {
  float3 origin, lookAt, up;

  // basis vectors of camera coordinate system
  float3 i, j, k;

  // default coordinate system: i=right, j=up, -k=depth
  CameraTransform() : origin(0, 0, 0), lookAt(0, 0, -1), up(0, 1, 0) {
    updateVectors();
  }

  CameraTransform(float3 origin, float3 lookAt, float3 up)
      : origin(origin), lookAt(lookAt), up(up) {
    updateVectors();
  }

  inline void updateVectors() {
    k = normalize(origin - lookAt);
    i = normalize(up.cross(k));
    j = k.cross(i);
  }
};

struct Camera {
  // renderer settings
  int samplesPerPixel = 4;
  int maxDepth = 10;

  // camera settings
  int width, height;
  mfloat VFoV;
  CameraTransform camTrans;

  mfloat widthF, heightF;
  float2 screenSize;
  mfloat aspectRatio;
  mfloat viewportHeight, viewportWidth;
  float2 viewportSize;
  float2 viewportCenter;

  Camera(int width, int height, mfloat VFoV, CameraTransform camTrans = {})
      : width(width),
        height(height),
        widthF(width),
        heightF(height),
        screenSize(height, width),
        VFoV(VFoV),
        camTrans(camTrans) {
    aspectRatio = widthF / heightF;
    auto focalLength = (camTrans.origin - camTrans.lookAt).length();
    auto theta = Deg2Rad(VFoV);
    auto h = tan(theta / 2);
    viewportHeight = 2 * h;
    // viewportHeight = 2 * h * focalLength;
    viewportWidth = viewportHeight * widthF / heightF;
    viewportSize = float2(viewportHeight, viewportWidth);
    float3 viewportU = camTrans.i * viewportWidth;
    float3 viewportV = camTrans.j * viewportHeight;
    viewportCenter = float2(0.5, 0.5);
  }

  inline Image render(const HittableList &scene, bool printLog = true) {
    Image image(height, width, 3);

    int lines_rendered = 0;
    omp_lock_t lines_rendered_mutex;
    if (printLog) omp_init_lock(&lines_rendered_mutex);

#pragma omp parallel for num_threads(16)
    for (int x = 0; x < height; x++) {
      for (int y = 0; y < width; y++) {
        ColorF3 colorSum(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; s++) {
          auto samplePos = getRandomSamplePos(x, y);
          auto ray = rayToScreenPos(samplePos / screenSize);
          colorSum += rayColor(ray, scene);
        }
        colorSum /= samplesPerPixel;
        image.setPixel(x, y, colorSum);
      }

      if (printLog) {
        omp_set_lock(&lines_rendered_mutex);
        lines_rendered++;
        print("Lines rendered:", lines_rendered, "/", height);
        omp_unset_lock(&lines_rendered_mutex);
      }
    }

    if (printLog) {
      print("all lines rendered.");
      omp_destroy_lock(&lines_rendered_mutex);
    }

    return image;
  }

  inline Ray rayToScreenPos(const float2 &screenPos) {
    // screenPos: (0, 1)^2
    // screenPosCentered: (-0.5, 0.5)^2
    float2 screenPosCentered = screenPos - viewportCenter;
    // a viewport plane at z = -1
    float2 worldPos = viewportSize * screenPosCentered;
    float3 dir = camTrans.i * worldPos.y() +   // right
                 camTrans.j * -worldPos.x() +  // up
                 camTrans.k * -1;              // depth
    // emit a ray from the origin
    return Ray{camTrans.origin, normalize(dir)};
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
      auto hit = result.ret;

      auto matResult = hit.material->scatter(ray, hit);
      // not absorbed
      if (matResult.success) {
        auto scatteredRay = matResult.ret;
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