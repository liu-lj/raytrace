#pragma once

#include <iostream>
#include <string>

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

template <typename T>
decltype(auto) lerp(T a, T b, float t) {
  return a * (1 - t) + b * t;
}