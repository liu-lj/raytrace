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
#include "Camera.hpp"

const int width = 600, height = 400;

int main(int argc, char **argv) {
  std::ios::sync_with_stdio(false);

  Image image(height, width, 3);
  HittableList scene(std::make_shared<Sphere>(0.5, float4(0, 0, -1, 1),
                                              ColorI4(255, 0, 0, 0)));
  Camera camera(width, height);
  camera.samplesPerPixel = 10;
  image = camera.render(scene);
  image.writePNG("test.png");
  print("image saved at", get_dir(argv[0]) + "/test.png");

  return 0;
}