#pragma warning(disable : 4819)

using mfloat = double;

#include <cstdlib>
#include <iostream>
#include <omp.h>

#include "Color.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"

const int width = 1920, height = 1080;

int main(int argc, char **argv) {
  std::ios::sync_with_stdio(false);

  // setup scene
  HittableList scene;
  auto groundMat = std::make_shared<Lambertian>(ColorF3(0.5, 0.5, 0.5));
  scene.add(
      std::make_shared<Sphere>(mfloat(1000), float3(0, -1000, 0), groundMat));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto chooseMat = RandFloat();
      float3 center(a + 0.9 * RandFloat(), 0.2, b + 0.9 * RandFloat());
      if ((center - float3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<Material> sphereMat;
        if (chooseMat < 0.8) {
          // diffuse
          auto albedo = RandomUnitVector() * RandomUnitVector();
          sphereMat = std::make_shared<Lambertian>(albedo);
          scene.add(std::make_shared<Sphere>(mfloat(0.2), center, sphereMat));
        } else if (chooseMat < 0.95) {
          // metal
          auto albedo = RandomUnitVector() / 2 + 0.5;
          auto fuzz = RandFloat(0, 0.5);
          sphereMat = std::make_shared<Metal>(albedo, fuzz);
          scene.add(std::make_shared<Sphere>(mfloat(0.2), center, sphereMat));
        } else {
          // glass
          sphereMat = std::make_shared<Dielectric>(mfloat(1.5));
          scene.add(std::make_shared<Sphere>(mfloat(0.2), center, sphereMat));
        }
      }
    }
  }

  auto mat1 = std::make_shared<Dielectric>(mfloat(1.5));
  scene.add(std::make_shared<Sphere>(mfloat(1.0), float3(0, 1, 0), mat1));

  auto mat2 = std::make_shared<Lambertian>(ColorF3(0.4, 0.2, 0.1));
  scene.add(std::make_shared<Sphere>(mfloat(1.0), float3(-4, 1, 0), mat2));

  auto mat3 = std::make_shared<Metal>(ColorF3(0.7, 0.6, 0.5), mfloat(0.0));
  scene.add(std::make_shared<Sphere>(mfloat(1.0), float3(4, 1, 0), mat3));

  // setup camera
  CameraTransform camTrans{
      float3(13, 2, 3),  // origin
      float3(0, 0, 0),   // lookAt
      float3(0, 1, 0)    // up
  };

  // defocus disk parameters
  mfloat angle = 2, foucsDist = 10, imageDist = 0.1;
  std::cin >> angle >> foucsDist >> imageDist;
  DefocusDisk ddisk{angle, foucsDist, imageDist, camTrans};

  Camera camera{width, height,
                20,  // Vertical FoV
                camTrans, ddisk};
  camera.samplesPerPixel = 4096;
  camera.maxDepth = 40;

  timeTest([&]() -> void {
    print("rendering...");

    // render
    bool printLog = camera.samplesPerPixel > 10;
    auto image = camera.render(scene, printLog);

    // save image
    image.linearToGamma();
    image.writePNG("test.png");
    print("image saved at", get_dir(argv[0]) + "/test.png");
  });

  return 0;
}