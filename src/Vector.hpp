#pragma once

#include <array>
#include <cmath>
#include <cstring>
#include <exception>
#include <string>

namespace MathUtils {
template <int n>
concept IsVectorDim = n > 1;

template <typename T, int n>
  requires IsVectorDim<n>
struct VectorBase {
  std::array<T, n> val;

#pragma region Constructors and basic operators
  // 空构造函数
  VectorBase() {}

  // 复制构造函数
  VectorBase(const VectorBase<T, n>& v) { val = v.val; }

  // 移动构造函数
  VectorBase(VectorBase<T, n>&& v) : val(std::move(v.val)) {}

  // 维度不相同的向量的复制构造函数
  template <typename T2, int n2>
  VectorBase(const VectorBase<T2, n2>& v) {
    int len = std::min(n, n2);
    for (int i = 0; i < len; i++) val[i] = static_cast<T>(v.val[i]);
  }

  // 构造函数，参数为向量的值
  template <typename T0, typename T1, typename... Ts>
  VectorBase(T0 arg0, T1 arg1, Ts... args) {
    static_assert(sizeof...(args) == n - 2,
                  "Number of arguments does not match type!");
    readVals({arg0, arg1, args...});
  }

  void readVals(std::initializer_list<T> vals) {
    int i = 0;
    for (auto v : vals) val[i++] = v;
  }

  T& operator[](int i) {
    if (i < 0 || i >= n) throw std::out_of_range("Index out of range!");
    return val[i];
  }

  template <typename... Ts>
  auto operator()(Ts... args) -> VectorBase<T, sizeof...(args)> {
    static_assert(sizeof...(args) == n,
                  "Number of arguments does not match type!");
    VectorBase<T, sizeof...(args)> res;
    std::initializer_list<int> indexs{args...};
    int i = 0;
    for (auto index : indexs) res.val[i++] = val[index];
    return res;
  }
#pragma endregion

#pragma region I/O functions
  std::string toString() const {
    std::stringstream ss;
    for (int i = 0; i < n - 1; i++) ss << val[i] << ", ";
    ss << val[n - 1];
    return ss.str();
  }

  friend std::istream& operator>>(std::istream& is, VectorBase<T, n>& v) {
    for (int i = 0; i < n; i++) is >> v.val[i];
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const VectorBase<T, n>& v) {
    for (int i = 0; i < n - 1; i++) os << v.val[i] << " ";
    os << v.val[n - 1];
    return os;
  }
#pragma endregion

#pragma region Math functions
  VectorBase<T, n> operator+(const VectorBase<T, n>& v) const {
    VectorBase<T, n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] + v.val[i];
    return res;
  }

  VectorBase<T, n> operator-(const VectorBase<T, n>& v) const {
    VectorBase<T, n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] - v.val[i];
    return res;
  }

  T operator*(const VectorBase<T, n>& v) const { return dot(v); }

  VectorBase<T, n>& operator+=(const VectorBase<T, n>& v) {
    for (int i = 0; i < n; i++) val[i] += v.val[i];
    return *this;
  }

  VectorBase<T, n>& operator-=(const VectorBase<T, n>& v) {
    for (int i = 0; i < n; i++) val[i] -= v.val[i];
    return *this;
  }

  VectorBase<T, n>& operator*=(const VectorBase<T, n>& v) {
    for (int i = 0; i < n; i++) val[i] *= v.val[i];
    return *this;
  }

  VectorBase<T, n> operator+(T x) const {
    VectorBase<T, n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] + x;
    return res;
  }

  VectorBase<T, n> operator-(T x) const {
    VectorBase<T, n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] - x;
    return res;
  }

  VectorBase<T, n> operator*(T x) const {
    VectorBase<T, n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] * x;
    return res;
  }

  VectorBase<T, n> operator/(T x) const {
    VectorBase<T, n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] / x;
    return res;
  }

  VectorBase<T, n>& operator+=(T x) {
    for (int i = 0; i < n; i++) val[i] += x;
    return *this;
  }

  VectorBase<T, n>& operator-=(T x) {
    for (int i = 0; i < n; i++) val[i] -= x;
    return *this;
  }

  VectorBase<T, n>& operator*=(T x) {
    for (int i = 0; i < n; i++) val[i] *= x;
    return *this;
  }

  VectorBase<T, n>& operator/=(T x) {
    for (int i = 0; i < n; i++) val[i] /= x;
    return *this;
  }

  T dot(const VectorBase<T, n>& v) const {
    T sum = 0;
    for (int i = 0; i < n; i++) sum += val[i] * v.val[i];
    return sum;
  }

  T length() const { return dot(*this); }

  VectorBase<T, n>& normalized() {
    T invLen = 1 / length();
    for (int i = 0; i < n; i++) val[i] *= invLen;
    return *this;
  }

  VectorBase<T, n>& safeNormalized() {
    T invLen = 1 / max(length(), 1e-3);
    for (int i = 0; i < n; i++) val[i] *= invLen;
    return *this;
  }
#pragma endregion
};

template <typename T, int n>
struct Vector : public VectorBase<T, n> {
  using VectorBase<T, n>::VectorBase;
};

template <typename T>
struct Vector<T, 2> : VectorBase<T, 2> {
  using VectorBase<T, 2>::VectorBase;
  T& x() { return this->val[0]; }
  T& y() { return this->val[1]; }
};

template <typename T>
struct Vector<T, 3> : public VectorBase<T, 3> {
  using VectorBase<T, 3>::VectorBase;
  T& x() { return this->val[0]; }
  T& y() { return this->val[1]; }
  T& z() { return this->val[2]; }
  T& r() { return this->val[0]; }
  T& g() { return this->val[1]; }
  T& b() { return this->val[2]; }
};

template <typename T>
struct Vector<T, 4> : public VectorBase<T, 4> {
  using VectorBase<T, 4>::VectorBase;
  Vector(VectorBase<T, 4>& v) : Vector(std::move(v)) {}
  Vector(VectorBase<T, 4>&& v) : VectorBase<T, 4>(v) {}
  T& x() { return this->val[0]; }
  T& y() { return this->val[1]; }
  T& z() { return this->val[2]; }
  T& w() { return this->val[3]; }
  T& r() { return this->val[0]; }
  T& g() { return this->val[1]; }
  T& b() { return this->val[2]; }
  T& a() { return this->val[3]; }
};

using int2 = Vector<int, 2>;
using float2 = Vector<float, 2>;
using double2 = Vector<double, 2>;

using int3 = Vector<int, 3>;
using float3 = Vector<float, 3>;
using double3 = Vector<double, 3>;

using int4 = Vector<int, 4>;
using float4 = Vector<float, 4>;

template <typename T, int n>
VectorBase<T, n>& normalize(const VectorBase<T, n>& v) {
  T invLen = 1 / v.length();
  VectorBase<T, n> res;
  for (int i = 0; i < n; i++) res.val[i] = v.val[i] * invLen;
  return res;
}

template <typename T, int n>
VectorBase<T, n>& safeNormalize(const VectorBase<T, n>& v) {
  T invLen = 1 / (v.length() + 1e-5);
  VectorBase<T, n> res;
  for (int i = 0; i < n; i++) res.val[i] = v.val[i] * invLen;
  return res;
}
}  // namespace MathUtils

using MathUtils::normalize;
using MathUtils::safeNormalize;

using MathUtils::Vector;

using MathUtils::double2;
using MathUtils::float2;
using MathUtils::int2;

using MathUtils::double3;
using MathUtils::float3;
using MathUtils::int3;

using MathUtils::float4;
using MathUtils::int4;