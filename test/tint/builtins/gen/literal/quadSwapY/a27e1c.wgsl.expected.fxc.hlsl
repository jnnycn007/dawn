SKIP: INVALID

//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint2 quadSwapY_a27e1c() {
  uint2 res = QuadReadAcrossY((1u).xx);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, quadSwapY_a27e1c());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint2 quadSwapY_a27e1c() {
  uint2 res = QuadReadAcrossY((1u).xx);
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, quadSwapY_a27e1c());
}

