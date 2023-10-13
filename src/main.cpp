// #define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable:4819)

#include <cstdlib>
#include <iostream>

// clang-format off
#include "Color.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Utils.hpp"
#include "Vector.hpp"
// clang-format on

using std::cin;
using std::cout;
using std::endl;

const int width = 800, height = 400;
const float widthF = width, heightF = height;
const float4 origin(0, 0, 0, 1);

int main() {
  Image img(height, width, 3);
  Sphere sphere(float4(0, 0, -1, 1), 1, ColorI4(255, 0, 0, 0));
  float aspectRatio = widthF / heightF;
  float viewportHeight = 2;
  float viewportWidth = aspectRatio * widthF / heightF;
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) {
      float u = (i + 0.5f) / widthF, v = (j + 0.5f) / heightF;
      float4 dir = float4(viewportWidth * (u - 0.5f),
                          viewportHeight * (v - 0.5f), -1, 0);
      Ray r(origin, dir);
      // ColorI3 c = rayTrace(r);
      ColorI3 c = ColorI3(25, 255 * (u + v) / 4, 255 * (u + v) / 2);
      if (sphere.intersect(r))
        c = sphere.color;
      img.setPixel(i, j, c);
    }
  img.writePNG("test.png");
  return 0;
}