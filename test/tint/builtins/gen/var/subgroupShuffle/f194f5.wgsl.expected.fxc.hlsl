SKIP: INVALID

//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint subgroupShuffle_f194f5() {
  uint arg_0 = 1u;
  uint arg_1 = 1u;
  uint res = WaveReadLaneAt(arg_0, arg_1);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, subgroupShuffle_f194f5());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint subgroupShuffle_f194f5() {
  uint arg_0 = 1u;
  uint arg_1 = 1u;
  uint res = WaveReadLaneAt(arg_0, arg_1);
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, subgroupShuffle_f194f5());
}

