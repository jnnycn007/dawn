//
// fragment_main
//

void distance_ac5535() {
  float res = 0.0f;
}

void fragment_main() {
  distance_ac5535();
}

//
// compute_main
//

void distance_ac5535() {
  float res = 0.0f;
}

[numthreads(1, 1, 1)]
void compute_main() {
  distance_ac5535();
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
};

struct vertex_main_outputs {
  float4 VertexOutput_pos : SV_Position;
};


void distance_ac5535() {
  float res = 0.0f;
}

VertexOutput vertex_main_inner() {
  VertexOutput v = (VertexOutput)0;
  v.pos = (0.0f).xxxx;
  distance_ac5535();
  VertexOutput v_1 = v;
  return v_1;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_2 = vertex_main_inner();
  vertex_main_outputs v_3 = {v_2.pos};
  return v_3;
}

