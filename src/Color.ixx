export module Color;

import Vector;

export {
  using ColorF3 = float3;
  using ColorI3 = int3;
  // using ColorU3 = Vector<unsigned char, 3>;

  // using ColorF4 = float4;
  using ColorI4 = int4;
  // using ColorU4 = Vector<unsigned char, 4>;

  inline ColorI3 ToColorI3(ColorF3 color) { return color * 255.999f; }
}