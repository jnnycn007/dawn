SKIP: INVALID

//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint4 quadBroadcast_2d0b7d() {
  uint4 arg_0 = (1u).xxxx;
  uint4 res = QuadReadLaneAt(arg_0, 1u);
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, quadBroadcast_2d0b7d());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint4 quadBroadcast_2d0b7d() {
  uint4 arg_0 = (1u).xxxx;
  uint4 res = QuadReadLaneAt(arg_0, 1u);
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, quadBroadcast_2d0b7d());
}

