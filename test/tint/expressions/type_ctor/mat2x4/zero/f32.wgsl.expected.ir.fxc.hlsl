
static float2x4 m = float2x4((0.0f).xxxx, (0.0f).xxxx);
RWByteAddressBuffer v : register(u0);
void v_1(uint offset, float2x4 obj) {
  v.Store4((offset + 0u), asuint(obj[0u]));
  v.Store4((offset + 16u), asuint(obj[1u]));
}

[numthreads(1, 1, 1)]
void f() {
  v_1(0u, m);
}

