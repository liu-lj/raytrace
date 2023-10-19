#pragma warning(disable : 4819)

using mfloat = float;

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
  auto matGround = std::make_shared<Lambertian>(ColorF3(0.8, 0.8, 0.0));
  auto matCenter = std::make_shared<Lambertian>(ColorF3(0.1, 0.2, 0.5));
  auto matLeft = std::make_shared<Dielectric>(mfloat(1.5));
  auto matRight = std::make_shared<Metal>(ColorF3(0.8, 0.6, 0.2), mfloat(1));
  scene.add(std::make_shared<Sphere>(100.0f, float3(0, -100.5, -1), matGround));
  scene.add(std::make_shared<Sphere>(0.5f, float3(0, 0, -1), matCenter));
  scene.add(std::make_shared<Sphere>(0.5f, float3(-1, 0, -1), matLeft));
  scene.add(std::make_shared<Sphere>(0.5f, float3(1, 0, -1), matRight));

  // setup camera
  Camera camera(width, height);
  camera.samplesPerPixel = 100;
  camera.maxDepth = 10;

  // render
  auto image = camera.render(scene);

  // save image
  // image.linearToGamma();
  image.writePNG("test.png");
  print("image saved at", get_dir(argv[0]) + "/test.png");

  return 0;
}