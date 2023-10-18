#pragma warning(disable : 4819)

#include <cstdlib>
#include <iostream>
#include <omp.h>

#include "Color.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Utils.hpp"
#include "Vector.hpp"

using std::cin;
using std::cout;
using std::endl;

const int width = 600, height = 400;
const float widthF = width, heightF = height;
const float aspectRatio = widthF / heightF;
// (x, y) in viewport: x: [-1, 1], y: [-aspectRatio, aspectRatio]
const float viewportHeight = 2;
const float viewportWidth = viewportHeight * widthF / heightF;
const float2 viewportSize(viewportHeight, viewportWidth);
const float2 viewportCenter(0.5, 0.5);
const float4 origin(0, 0, 0, 1);

int main(int argc, char** argv) {
  std::ios::sync_with_stdio(false);

  Image image(height, width, 3);
  Sphere sphere{0.5, float4(0, 0, -1, 1), ColorI4(255, 0, 0, 0)};

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
      // background color (sky color)
      ColorI3 c(255 * x / height, 255 * x / height, 255);
      float blend =
          0.5 * (static_cast<float3>(dir(0, 1, 2).safeNormalized()).y() + 1.0);
      c = lerp(ColorI3(255, 255, 255), ColorI3(0, 0, 255), blend);
      // ray trace
      auto result = sphere.hit(ray, 0, INF);
      result.ok([=, &c](HitRecord hit) {
        if (hit.frontFace)
          // auto t1 = t.first;
          // auto pos = ray(t1)(0, 1, 2);
          // auto normal = (pos - sphere.center(0, 1, 2)).safeNormalized();
          c = ToColorI3((hit.normal + 1) * 0.5f);
      });
      image.setPixel(x, y, c);
    }

    omp_set_lock(&lines_rendered_mutex);
    lines_rendered++;
    print("Lines rendered:", lines_rendered, "/", height);
    omp_unset_lock(&lines_rendered_mutex);
  }

  print("all lines rendered.");
  omp_destroy_lock(&lines_rendered_mutex);

  image.writePNG("test.png");
  print("image saved at", get_dir(argv[0]) + "/test.png");

  return 0;
}