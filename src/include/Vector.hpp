#pragma once

#include <array>
#include <cmath>
#include <cstring>
#include <exception>
#include <string>

#include "Utils.hpp"
#include "MathUtils.hpp"

namespace VectorDef {
// 限制向量的维度至少为 2
template <size_t n>
concept IsVectorDim = n > 1;

template <typename T, size_t n>
  requires IsVectorDim<n>
struct VectorBase {
  std::array<T, n> val;

#pragma region Constructors and basic operators
  // 空构造函数
  VectorBase() {}

  // 复制构造函数
  VectorBase(const VectorBase<T, n> &v) { val = v.val; }

  // 移动构造函数
  VectorBase(VectorBase<T, n> &&v) : val(std::move(v.val)) {}

  // 显式使用默认的复制赋值运算符
  VectorBase<T, n> &operator=(const VectorBase<T, n> &v) = default;

  // 显式使用默认的移动赋值运算符
  VectorBase<T, n> &operator=(VectorBase<T, n> &&v) = default;

  // 维度不相同的向量的复制构造函数
  template <typename T2, size_t n2>
  VectorBase(const VectorBase<T2, n2> &v) {
    size_t len = std::min(n, n2);
    for (size_t i = 0; i < len; i++) val[i] = static_cast<T>(v.val[i]);
  }

  // 构造函数，参数为向量的值
  template <typename T0, typename T1, typename... Ts>
  VectorBase(T0 arg0, T1 arg1, Ts... args) {
    static_assert(sizeof...(args) == n - 2,
                  "Number of arguments does not match type!");
    readVals(
        {static_cast<T>(arg0), static_cast<T>(arg1), static_cast<T>(args)...});
  }

  void readVals(std::initializer_list<T> vals) {
    size_t i = 0;
    for (auto v : vals) val[i++] = v;
  }

  // 带边界检查的下标访问
  T &operator[](int i) {
    if (i < 0 || i >= n) throw std::out_of_range("Index out of range!");
    return val[i];
  }

  // 带边界检查的下标访问
  const T &operator[](int i) const {
    if (i < 0 || i >= n) throw std::out_of_range("Index out of range!");
    return val[i];
  }

  template <typename... Ts>
  auto operator()(Ts... args) const -> VectorBase<T, sizeof...(args)> {
    constexpr size_t len = sizeof...(args);
    VectorBase<T, len> res;
    std::initializer_list<size_t> indexs{static_cast<size_t>(args)...};
    size_t i = 0;
    for (auto index : indexs) res.val[i++] = val[index];
    return res;
  }

  static VectorBase<T, n> random(T min, T max) {
    VectorBase<T, n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = RandFloat(min, max);
    return res;
  }
#pragma endregion

#pragma region I/O functions
  std::string toString() const {
    std::stringstream ss;
    for (size_t i = 0; i < n - 1; i++) ss << val[i] << " ";
    ss << val[n - 1];
    return ss.str();
  }

  friend std::istream &operator>>(std::istream &is, VectorBase<T, n> &v) {
    for (size_t i = 0; i < n; i++) is >> v.val[i];
    return is;
  }

  friend std::ostream &operator<<(std::ostream &os, const VectorBase<T, n> &v) {
    for (size_t i = 0; i < n - 1; i++) os << v.val[i] << " ";
    os << v.val[n - 1];
    return os;
  }
#pragma endregion

#pragma region Math functions
  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> operator+(T2 x) const {
    VectorBase<T, n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] + x;
    return res;
  }

  template <typename T2>
  auto operator+(const VectorBase<T2, n> &x) const
      -> VectorBase<decltype(std::declval<T>() * std::declval<T2>()), n> {
    VectorBase<decltype(std::declval<T>() * std::declval<T2>()), n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] + x.val[i];
    return res;
  }

  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> operator-(T2 x) const {
    VectorBase<T, n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] - x;
    return res;
  }

  template <typename T2>
  auto operator-(const VectorBase<T2, n> &x) const
      -> VectorBase<decltype(std::declval<T>() * std::declval<T2>()), n> {
    VectorBase<decltype(std::declval<T>() * std::declval<T2>()), n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] - x.val[i];
    return res;
  }

  // multiply by a scalar
  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> operator*(T2 x) const {
    VectorBase<T, n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] * x;
    return res;
  }

  // multiply by a vector element-wise
  template <typename T2>
  auto operator*(const VectorBase<T2, n> &x) const
      -> VectorBase<decltype(std::declval<T>() * std::declval<T2>()), n> {
    VectorBase<decltype(std::declval<T>() * std::declval<T2>()), n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] * x.val[i];
    return res;
  }

  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> operator/(T2 x) const {
    VectorBase<T, n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] / x;
    return res;
  }

  // divide by a vector element-wise
  template <typename T2>
  auto operator/(const VectorBase<T2, n> &x) const
      -> VectorBase<decltype(std::declval<T>() / std::declval<T2>()), n> {
    VectorBase<decltype(std::declval<T>() / std::declval<T2>()), n> res;
    for (size_t i = 0; i < n; i++) res.val[i] = val[i] / x.val[i];
    return res;
  }

  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> &operator+=(T2 x) {
    for (size_t i = 0; i < n; i++) val[i] += x;
    return *this;
  }

  VectorBase<T, n> &operator+=(const VectorBase<T, n> &v) {
    for (size_t i = 0; i < n; i++) val[i] += v.val[i];
    return *this;
  }

  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> &operator-=(T2 x) {
    for (size_t i = 0; i < n; i++) val[i] -= x;
    return *this;
  }

  VectorBase<T, n> &operator-=(const VectorBase<T, n> &v) {
    for (size_t i = 0; i < n; i++) val[i] -= v.val[i];
    return *this;
  }

  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> &operator*=(T2 x) {
    for (size_t i = 0; i < n; i++) val[i] *= x;
    return *this;
  }

  VectorBase<T, n> &operator*=(const VectorBase<T, n> &v) {
    for (size_t i = 0; i < n; i++) val[i] *= v.val[i];
    return *this;
  }

  template <typename T2>
    requires std::is_arithmetic_v<T2>
  VectorBase<T, n> &operator/=(T2 x) {
    for (size_t i = 0; i < n; i++) val[i] /= x;
    return *this;
  }

  T dot(const VectorBase<T, n> &v) const {
    T sum = 0;
    for (size_t i = 0; i < n; i++) sum += val[i] * v.val[i];
    return sum;
  }

  T pow() const { return dot(*this); }
  decltype(auto) length() const { return sqrt(pow()); }

  // 浮点型向量的归一化 (整型向量不支持此操作)
  template <typename U = T>
    requires std::floating_point<U>
  VectorBase<T, n> &normalize() {
    auto len2 = pow();
    if (std::abs(len2 - 1) < 1e-3) return *this;
    mfloat invLen = 1 / sqrt(len2);
    for (size_t i = 0; i < n; i++) val[i] *= invLen;
    return *this;
  }

  // 浮点型向量的安全归一化 (整型向量不支持此操作)
  template <typename U = T>
    requires std::floating_point<U>
  VectorBase<T, n> &safeNormalize() {
    auto len2 = pow();
    if (std::abs(len2 - 1) < 1e-3) return *this;
    auto invLen = 1 / std::max(sqrt(len2), static_cast<decltype(len2)>(1e-3));
    for (size_t i = 0; i < n; i++) val[i] *= invLen;
    return *this;
  }
#pragma endregion
};

#pragma region Derived types
template <typename T, size_t n>
struct Vector : public VectorBase<T, n> {
  using VectorBase<T, n>::VectorBase;
  Vector(const VectorBase<T, n> &other) : VectorBase<T, n>(other) {}
  Vector(VectorBase<T, n> &&other) : VectorBase<T, n>(std::move(other)) {}
};

template <typename T>
struct Vector<T, 2> : VectorBase<T, 2> {
  using VectorBase<T, 2>::VectorBase;
  Vector(const VectorBase<T, 2> &other) : VectorBase<T, 2>(other) {}
  Vector(VectorBase<T, 2> &&other) : VectorBase<T, 2>(std::move(other)) {}
  T &x() { return this->val[0]; }
  T &y() { return this->val[1]; }
};

template <typename T>
struct Vector<T, 3> : public VectorBase<T, 3> {
  using VectorBase<T, 3>::VectorBase;
  Vector(const VectorBase<T, 3> &other) : VectorBase<T, 3>(other) {}
  Vector(VectorBase<T, 3> &&other) : VectorBase<T, 3>(std::move(other)) {}

  Vector<T, 3> cross(const Vector<T, 3> &other) {
    Vector<T, 3> res;
    res.val[0] = this->val[1] * other.val[2] - this->val[2] * other.val[1];
    res.val[1] = this->val[2] * other.val[0] - this->val[0] * other.val[2];
    res.val[2] = this->val[0] * other.val[1] - this->val[1] * other.val[0];
    return res;
  }

  T &x() { return this->val[0]; }
  T &y() { return this->val[1]; }
  T &z() { return this->val[2]; }
  T &r() { return this->val[0]; }
  T &g() { return this->val[1]; }
  T &b() { return this->val[2]; }
};

template <typename T>
struct Vector<T, 4> : public VectorBase<T, 4> {
  using VectorBase<T, 4>::VectorBase;
  Vector(const VectorBase<T, 4> &other) : VectorBase<T, 4>(other) {}
  Vector(VectorBase<T, 4> &&other) : VectorBase<T, 4>(std::move(other)) {}
  T &x() { return this->val[0]; }
  T &y() { return this->val[1]; }
  T &z() { return this->val[2]; }
  T &w() { return this->val[3]; }
  T &r() { return this->val[0]; }
  T &g() { return this->val[1]; }
  T &b() { return this->val[2]; }
  T &a() { return this->val[3]; }
};
#pragma endregion

#pragma region Type aliases
using int2 = Vector<int, 2>;
using float2 = Vector<mfloat, 2>;
using double2 = Vector<double, 2>;

using int3 = Vector<int, 3>;
using float3 = Vector<mfloat, 3>;
using double3 = Vector<double, 3>;

using int4 = Vector<int, 4>;
using float4 = Vector<mfloat, 4>;
#pragma endregion

#pragma region Non-member functions
// 浮点型向量的归一化 (整型向量不支持此操作)
template <typename T, size_t n>
  requires std::floating_point<T>
VectorBase<T, n> normalize(const VectorBase<T, n> &v) {
  auto len2 = v.pow();
  if (std::abs(len2 - 1) < 1e-3) return v;
  auto invLen = 1 / sqrt(len2);
  VectorBase<T, n> res;
  for (size_t i = 0; i < n; i++) res.val[i] = v.val[i] * invLen;
  return res;
}

// 浮点型向量的安全归一化 (整型向量不支持此操作)
template <typename T, size_t n>
  requires std::floating_point<T>
VectorBase<T, n> safeNormalize(const VectorBase<T, n> &v) {
  auto len2 = v.pow();
  if (std::abs(len2 - 1) < 1e-3) return VectorBase<T, n>(v);
  auto invLen = 1 / std::max(sqrt(len2), static_cast<decltype(len2)>(1e-3));
  VectorBase<T, n> res;
  for (size_t i = 0; i < n; i++) res.val[i] = v.val[i] * invLen;
  return res;
}
#pragma endregion
}  // namespace VectorDef

using VectorDef::normalize;
using VectorDef::safeNormalize;

using VectorDef::Vector;

using VectorDef::double2;
using VectorDef::float2;
using VectorDef::int2;

using VectorDef::double3;
using VectorDef::float3;
using VectorDef::int3;

using VectorDef::float4;
using VectorDef::int4;

template <typename T, size_t n>
void AssertVectorNormalized(const Vector<T, n> &v) {
  if (std::abs(v.pow() - 1) < 1e-3) print("Vector not normalized!");
}

template <typename T, size_t n>
Vector<T, n> abs(const Vector<T, n> &v) {
  Vector<T, n> res;
  for (size_t i = 0; i < n; i++) res.val[i] = std::abs(v.val[i]);
  return res;
}

template <typename T, size_t n>
Vector<T, n> clamp(const Vector<T, n> &v, T min, T max) {
  Vector<T, n> res;
  for (size_t i = 0; i < n; i++) res.val[i] = std::clamp(v.val[i], min, max);
  return res;
}

template <typename T, size_t n>
Vector<T, n> saturate(const Vector<T, n> &v) {
  return clamp<T, n>(v, 0, 1);
}

// extend functions
inline float3 RandomInUnitSphere() {
  while (true) {
    auto vec = float3::random(-1, 1);
    auto pow = vec.pow();
    if (pow < 1 && pow > 1e-3) return vec;
  }
}

inline float3 RandomUnitVector() { return normalize(RandomInUnitSphere()); }

inline float3 RandomOnHemisphere(const float3 &normal) {
  auto inUnitSphere = RandomInUnitSphere();
  if (inUnitSphere.dot(normal) > 0)
    return inUnitSphere;
  else
    return inUnitSphere * -1;
}

inline float2 RandomInUnitDisk() {
  while (true) {
    float2 vec(RandFloat(-1, 1), RandFloat(-1, 1));
    auto pow = vec.pow();
    if (pow <= 1) return vec;
  }
}

inline float3 ReflectedVector(const float3 &rayIn, const float3 &normal) {
  return safeNormalize(rayIn - normal * 2 * rayIn.dot(normal));
}

inline float3 RefractedVector(const float3 &rayIn,            // 入射向量
                              const float3 &normal,           // 表面法向量
                              mfloat relativeRefractiveIndex  // 相对折射率
) {
  auto cosTheta = std::min(-rayIn.dot(normal), mfloat(1));
  auto refractedPrep = (rayIn + normal * cosTheta) * relativeRefractiveIndex;
  auto refractedPara = normal * -sqrt(fabs(1 - refractedPrep.pow()));
  return safeNormalize(refractedPrep + refractedPara);
}