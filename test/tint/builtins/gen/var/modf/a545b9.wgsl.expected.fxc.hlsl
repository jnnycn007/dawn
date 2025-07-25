SKIP: INVALID

//
// fragment_main
//
struct modf_result_vec2_f16 {
  vector<float16_t, 2> fract;
  vector<float16_t, 2> whole;
};


void modf_a545b9() {
  vector<float16_t, 2> arg_0 = (float16_t(-1.5h)).xx;
  vector<float16_t, 2> v = (float16_t(0.0h)).xx;
  modf_result_vec2_f16 res = {modf(arg_0, v), v};
}

void fragment_main() {
  modf_a545b9();
}

//
// compute_main
//
struct modf_result_vec2_f16 {
  vector<float16_t, 2> fract;
  vector<float16_t, 2> whole;
};


void modf_a545b9() {
  vector<float16_t, 2> arg_0 = (float16_t(-1.5h)).xx;
  vector<float16_t, 2> v = (float16_t(0.0h)).xx;
  modf_result_vec2_f16 res = {modf(arg_0, v), v};
}

[numthreads(1, 1, 1)]
void compute_main() {
  modf_a545b9();
}

//
// vertex_main
//
struct modf_result_vec2_f16 {
  vector<float16_t, 2> fract;
  vector<float16_t, 2> whole;
};

struct VertexOutput {
  float4 pos;
};

struct vertex_main_outputs {
  float4 VertexOutput_pos : SV_Position;
};


void modf_a545b9() {
  vector<float16_t, 2> arg_0 = (float16_t(-1.5h)).xx;
  vector<float16_t, 2> v = (float16_t(0.0h)).xx;
  modf_result_vec2_f16 res = {modf(arg_0, v), v};
}

VertexOutput vertex_main_inner() {
  VertexOutput v_1 = (VertexOutput)0;
  v_1.pos = (0.0f).xxxx;
  modf_a545b9();
  VertexOutput v_2 = v_1;
  return v_2;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_3 = vertex_main_inner();
  vertex_main_outputs v_4 = {v_3.pos};
  return v_4;
}

