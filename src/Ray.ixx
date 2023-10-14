export module Ray;

import Vector;

export {
  struct Ray {
    float4 origin, direction;

    inline float4 at(float t) const { return origin + direction * t; }
    inline float4 operator()(float t) const { return at(t); }
  };
}