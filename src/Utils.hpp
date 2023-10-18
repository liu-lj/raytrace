#pragma once

#include <limits>
#include <algorithm>
#include <iostream>
#include <string>
#include <functional>

template <typename T0, typename... Ts>
inline void print(T0 t0, Ts... ts) {
  std::cout << t0;
  if constexpr (sizeof...(ts) > 0) {
    std::cout << ' ';
    print(ts...);
  } else
    std::cout << std::endl;
}

// 仅用于处理 int main(int argc, char** argv) 的 argv[0]
std::string get_dir(std::string path) {
  std::replace(path.begin(), path.end(), '\\', '/');
  int pos = path.find_last_of('/');
  return path.substr(0, pos);
}

template <typename T, typename T2>
  requires std::is_arithmetic_v<T2>
decltype(auto) lerp(T a, T b, T2 t) {
  return a * (1 - t) + b * t;
}

template <typename T>
struct Result {
  bool success;
  T returnVal;
  Result() : success(false) {}
  Result(T returnVal) : success(true), returnVal(returnVal) {}
  inline void ok(std::function<void(T)> func) {
    if (success) func(returnVal);
  }
  inline void fail(std::function<void()> func) {
    if (!success) func();
  }
};

constexpr const float PI = 3.1415927f;
constexpr const float INF = std::numeric_limits<float>().infinity();

inline float Deg2Rad(float degrees) {
  return degrees * PI / 180;
}