export module Utils;

import <iostream>;
import <string>;
import <algorithm>;
import <functional>;

export {
  template <typename T0, typename... Ts>
  inline void print(T0 t0, Ts... ts) {
    std::cout << t0;
    if constexpr (sizeof...(ts) > 0) {
      std::cout << ' ';
      print(ts...);
    } else
      std::cout << std::endl;
  }

  // ONLY can be used to process argv[0] in int main(int argc, char** argv)
  std::string get_dir(std::string path) {
    std::replace(path.begin(), path.end(), '\\', '/');
    auto pos = path.find_last_of('/');
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
    void ok(std::function<void(T)> func) {
      if (success) func(returnVal);
    }
    void fail(std::function<void()> func) {
      if (!success) func();
    }
  };
}