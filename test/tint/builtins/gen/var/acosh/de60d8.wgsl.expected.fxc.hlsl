SKIP: INVALID

//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
vector<float16_t, 4> acosh_de60d8() {
  vector<float16_t, 4> arg_0 = (float16_t(1.54296875h)).xxxx;
  vector<float16_t, 4> v = arg_0;
  vector<float16_t, 4> res = log((v + sqrt(((v * v) - (float16_t(1.0h)).xxxx))));
  return res;
}

void fragment_main() {
  prevent_dce.Store<vector<float16_t, 4> >(0u, acosh_de60d8());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
vector<float16_t, 4> acosh_de60d8() {
  vector<float16_t, 4> arg_0 = (float16_t(1.54296875h)).xxxx;
  vector<float16_t, 4> v = arg_0;
  vector<float16_t, 4> res = log((v + sqrt(((v * v) - (float16_t(1.0h)).xxxx))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store<vector<float16_t, 4> >(0u, acosh_de60d8());
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
  vector<float16_t, 4> prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation vector<float16_t, 4> VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


vector<float16_t, 4> acosh_de60d8() {
  vector<float16_t, 4> arg_0 = (float16_t(1.54296875h)).xxxx;
  vector<float16_t, 4> v = arg_0;
  vector<float16_t, 4> res = log((v + sqrt(((v * v) - (float16_t(1.0h)).xxxx))));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_1 = (VertexOutput)0;
  v_1.pos = (0.0f).xxxx;
  v_1.prevent_dce = acosh_de60d8();
  VertexOutput v_2 = v_1;
  return v_2;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_3 = vertex_main_inner();
  vertex_main_outputs v_4 = {v_3.prevent_dce, v_3.pos};
  return v_4;
}

