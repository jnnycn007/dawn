SKIP: INVALID


static float16_t t = float16_t(0.0h);
matrix<float16_t, 3, 2> m() {
  t = (t + float16_t(1.0h));
  return matrix<float16_t, 3, 2>(vector<float16_t, 2>(float16_t(1.0h), float16_t(2.0h)), vector<float16_t, 2>(float16_t(3.0h), float16_t(4.0h)), vector<float16_t, 2>(float16_t(5.0h), float16_t(6.0h)));
}

void f() {
  float3x2 v = float3x2(m());
}

[numthreads(1, 1, 1)]
void unused_entry_point() {
}

