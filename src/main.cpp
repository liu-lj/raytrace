#pragma warning(disable : 4819)

#include <cstdlib>
#include <iostream>

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
  float viewportHeight = 2;  // x = [-1, 1], y = [-aspectRatio, aspectRatio]
  float viewportWidth = viewportHeight * widthF / heightF;

#pragma omp parallel num_threads(16)
#pragma omp parallel for
  for (int x = 0; x < height; x++)
    for (int y = 0; y < width; y++) {
      float pixel_h = (x + 0.5f) / heightF, pixel_w = (y + 0.5f) / widthF;
      float4 dir = float4(viewportWidth * (pixel_w - 0.5f),
                          viewportHeight * (pixel_h - 0.5f), -1, 0);
      Ray r(origin, dir);
      // ColorI3 c = rayTrace(r);
      ColorI3 c(25, 255 * (pixel_w + pixel_h) / 4, 255 * (pixel_w + pixel_h) / 2);
      c = ColorI3(100, 255 * x / heightF, 255 * y / widthF);
      // if (sphere.intersect(r)) c = sphere.color;
      image.setPixel(x, y, c);
    }

  image.writePNG("test.png");
  return 0;
}