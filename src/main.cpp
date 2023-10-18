#pragma warning(disable : 4819)

#include <cstdlib>
#include <iostream>
#include <omp.h>

#include "Color.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"

const int width = 600, height = 400;

int main(int argc, char **argv) {
  std::ios::sync_with_stdio(false);

  // setup scene
  HittableList scene;
  auto material_ground = std::make_shared<Lambertian>(ColorF3(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<Lambertian>(ColorF3(0.7, 0.3, 0.3));
  auto material_left = std::make_shared<Metal>(ColorF3(0.8, 0.8, 0.8));
  auto material_right = std::make_shared<Metal>(ColorF3(0.8, 0.6, 0.2));
  scene.add(std::make_shared<Sphere>(100.0f, float4(0, -100.5, -1, 1), material_ground));
  scene.add(std::make_shared<Sphere>(0.5f, float4(0, 0, -1, 1), material_center));
  scene.add(std::make_shared<Sphere>(0.5f, float4(-1, 0, -1, 1), material_left));
  scene.add(std::make_shared<Sphere>(0.5f, float4(1, 0, -1, 1), material_right));

  // setup camera
  Camera camera(width, height);
  camera.samplesPerPixel = 50;
  camera.maxDepth = 10;

  // render
  auto image = camera.render(scene);

  // save image
  image.linearToGamma();
  image.writePNG("test.png");
  print("image saved at", get_dir(argv[0]) + "/test.png");

  return 0;
}