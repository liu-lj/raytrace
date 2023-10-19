#pragma once

#include <functional>

template <typename T>
  requires(!std::is_same_v<T, bool>)
struct Result {
  bool success;
  T returnVal;
  Result() : success(false) {}
  Result(bool success) : success(success) {}
  Result(T returnVal) : success(true), returnVal(returnVal) {}
  inline void ok(std::function<void(T)> func) {
    if (success) func(returnVal);
  }
  inline void fail(std::function<void()> func) {
    if (!success) func();
  }
};