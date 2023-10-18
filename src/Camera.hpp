#pragma once

#include <omp.h>

#include "Hittable.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Image.hpp"

struct Camera {
  static const float2 viewportCenter;
  static const float4 origin;

  int width, height;
  float widthF, heightF;
  float aspectRatio;
  float viewportHeight, viewportWidth;
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
        // pixelPos: (0, 1)^2
        float2 pixelPos((x + 0.5f) / heightF, (y + 0.5f) / widthF);
        // pixelPosCentered: (-0.5, 0.5)^2
        float2 pixelPosCentered = pixelPos - viewportCenter;
        // a viewport plane at z = -1
        float2 worldPos = viewportSize * pixelPosCentered;
        // dir coordinates: x: left, y: up, -z: depth
        float4 dir = float4(worldPos.y(), -worldPos.x(), -1, 0);
        // emit a ray from the origin
        Ray ray{origin, dir};

        ColorI3 c = rayColor(ray, scene);

        image.setPixel(x, y, c);
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

  inline ColorI3 rayColor(const Ray &ray, const Hittable &scene) const {
    // background color (sky color)
    float blend =
        0.5 *
        (static_cast<float3>(ray.direction(0, 1, 2).safeNormalized()).y() +
         1.0);
    ColorI3 c = lerp(ColorI3(255, 255, 255), ColorI3(0, 0, 255), blend);
    // ray trace
    auto result = scene.hit(ray, Interval(0, INF));
    result.ok([=, &c](HitRecord hit) {
      if (hit.frontFace)
        c = ToColorI3((hit.normal + 1) * 0.5f);
    });
    return c;
  }
};

const float2 Camera::viewportCenter = float2(0.5, 0.5);
const float4 Camera::origin = float4(0, 0, 0, 1);
