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

const int width = 800, height = 400;
const float widthF = width, heightF = height;
const float4 origin(0, 0, 0, 1);

int main() {
  Image image(height, width, 3);
  Sphere sphere(float4(0, 0, -1, 1), 1, ColorI4(255, 0, 0, 0));

  float aspectRatio = widthF / heightF;
  float viewportHeight = 2;  // x: [-1, 1], y: [-aspectRatio, aspectRatio]
  float viewportWidth = viewportHeight * widthF / heightF;
  float2 viewportSize(viewportHeight, viewportWidth);
  float2 viewport_center(0.5, 0.5);

  int lines_rendered = 0;
  omp_lock_t lines_rendered_mutex;
  omp_init_lock(&lines_rendered_mutex);

#pragma omp parallel for num_threads(16)
  for (int x = 0; x < height; x++) {
    for (int y = 0; y < width; y++) {
      // pixel_h, pixel_w: (0, 1)
      // float pixel_h = (x + 0.5f) / heightF, pixel_w = (y + 0.5f) / widthF;
      float2 pixel_pos((x + 0.5f) / heightF, (y + 0.5f) / widthF);
      float2 pixel_pos_centered = pixel_pos - viewport_center;
      // float2 world_pos = viewportSize * pixel_pos_centered;
      // float4 dir = float4(world_pos.y(), world_pos.x(), -1, 0);
      // Ray r(origin, dir);
      // ColorI3 c = rayTrace(r);
      ColorI3 c;
      c = ColorI3(100, 255 * x / heightF, 255 * y / widthF);
      // if (sphere.intersect(r)) c = sphere.color;
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
  print("image saved.");

  return 0;
}