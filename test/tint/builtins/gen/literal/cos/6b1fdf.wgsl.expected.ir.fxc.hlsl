//
// fragment_main
//

void cos_6b1fdf() {
  float3 res = (1.0f).xxx;
}

void fragment_main() {
  cos_6b1fdf();
}

//
// compute_main
//

void cos_6b1fdf() {
  float3 res = (1.0f).xxx;
}

[numthreads(1, 1, 1)]
void compute_main() {
  cos_6b1fdf();
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


void cos_6b1fdf() {
  float3 res = (1.0f).xxx;
}

VertexOutput vertex_main_inner() {
  VertexOutput v = (VertexOutput)0;
  v.pos = (0.0f).xxxx;
  cos_6b1fdf();
  VertexOutput v_1 = v;
  return v_1;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_2 = vertex_main_inner();
  vertex_main_outputs v_3 = {v_2.pos};
  return v_3;
}

