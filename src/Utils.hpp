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

std::string get_dir(std::string path) {
  std::replace(path.begin(), path.end(), '\\', '/');
  int pos = path.find_last_of('/');
  return path.substr(0, pos);
}