#define _CRT_SECURE_NO_WARNINGS

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
// const float4 origin(0, 0, 0, 1);

#include <windows.h>
#include <sysinfoapi.h>
#include <array>

struct Movable {
  std::array<int, 10> data;
  Movable() {}
  Movable(Movable& other) : data(std::move(other.data)) {
  }
  Movable(const Movable& other) : data(std::move(other.data)) {
  }
  Movable(Movable&& other) : data(std::move(other.data)) {
  }
  // Movable& operator=(const Movable& other) {
  //   data = other.data;
  //   return *this;
  // }
  // Movable& operator=(Movable&& other) {
  //   data = other.data;
  //   // other.data = {};
  //   return *this;
  // }
};

struct Copy {
  int data[10];
  Copy() {}
  Copy(const Copy& other) { memcpy(data, other.data, sizeof(int) * 10); }
  Copy(Copy&& other) { memcpy(data, other.data, sizeof(int) * 10); }
  Copy& operator=(const Copy& other) { memcpy(data, other.data, sizeof(int) * 10); return *this; }
  Copy& operator=(Copy&& other) { memcpy(data, other.data, sizeof(int) * 10); return *this; }
};

struct Copy2 {
  std::array<int, 10> data;
  Copy2() {}
  Copy2(const Copy2& other) { data = other.data; }
  Copy2(Copy2&& other) { data = other.data; }
  Copy2& operator=(const Copy2& other) { data = other.data; return *this; }
  Copy2& operator=(Copy2&& other) { data = other.data; return *this; }
};

template <class T>
void test() {
  T a;
  for (int i = 0; i < 10; i++) a.data[i] = i;
  auto time = GetTickCount();
  for (int i = 0; i < 100000000; i++) {
    T b(a);
    a = T(b);
  }
  print("time interval:", GetTickCount() - time);
}

int main() {
  test<Movable>();
  test<Copy>();
  test<Copy2>();
   int4 a(1, 2, 3, 4), b(2, 3, 4, 5);
  // auto apb = a + b;
  // print("a", a, "\nb", b, "\napb", apb);
  // // int4 force = (int4)apb;
  // int4 force = apb;
  // std::move(apb);
  // print("force", force);
  cin >> a[0];
  // Image img(height, width, 3);
  // Sphere sphere(float4(0, 0, -1, 1), 1, ColorI4(255, 0, 0));
  // float aspectRatio = widthF / heightF;
  // float viewportHeight = 2;
  // float viewportWidth = aspectRatio * widthF / heightF;
  // for (int i = 0; i < width; i++)
  //   for (int j = 0; j < height; j++) {
  //     float u = (i + 0.5f) / widthF, v = (j + 0.5f) / heightF;
  //     float4 dir = float4(viewportWidth * (u - 0.5f), viewportHeight * (v -
  //     0.5f), -1, 0); Ray r(origin, dir);
  //     // ColorI3 c = rayTrace(r);
  //     ColorI3 c = ColorI3(25, 255 * (u + v) / 4, 255 * (u + v) / 2);
  //     if (sphere.intersect(r))
  //       c = sphere.color;
  //     img.setPixel(i, j, c);
  //   }
  // img.writePNG("test.png");
  // system("pause");
  return 0;
}