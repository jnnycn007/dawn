SKIP: INVALID


[numthreads(1, 1, 1)]
void f() {
  float16_t a = float16_t(1.0h);
  float16_t b = float16_t(2.0h);
  float16_t v = (a / b);
  float16_t r = (a - ((((v < float16_t(0.0h))) ? (ceil(v)) : (floor(v))) * b));
}

