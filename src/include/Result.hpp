#pragma once

#include <functional>

template <typename T>
  requires(!std::is_same_v<T, bool>)
struct Result {
  bool success;
  T ret;
  Result() : success(false) {}
  Result(bool success) : success(success) {}
  Result(T ret) : success(true), ret(ret) {}
};