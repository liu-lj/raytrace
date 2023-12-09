#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <functional>

template <typename T0, typename... Ts>
void print(T0 t0, Ts... ts) {
  std::cout << t0;
  if constexpr (sizeof...(ts) > 0) {
    std::cout << ' ';
    print(ts...);
  } else
    std::cout << std::endl;
}

// 仅用于处理 int main(int argc, char** argv) 的 argv[0]
inline std::string get_dir(std::string path) {
  std::replace(path.begin(), path.end(), '\\', '/');
  int pos = path.find_last_of('/');
  return path.substr(0, pos);
}

inline long long timeTest(std::function<void()> func, bool printLog = true) {
  auto start = std::chrono::steady_clock::now();
  func();
  auto end = std::chrono::steady_clock::now();
  auto duration = end - start;
  print("Time elapsed in", duration.count(), "ns, about",
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(),
        "ms");
  return duration.count();
}