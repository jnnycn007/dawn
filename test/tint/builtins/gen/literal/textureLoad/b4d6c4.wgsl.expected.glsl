//
// fragment_main
//
#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  vec4 inner;
} v;
layout(binding = 1, rg32f) uniform highp image2D f_arg_0;
vec4 textureLoad_b4d6c4() {
  vec4 res = imageLoad(f_arg_0, ivec2(min(uvec2(1u), (uvec2(imageSize(f_arg_0)) - uvec2(1u)))));
  return res;
}
void main() {
  v.inner = textureLoad_b4d6c4();
}
//
// compute_main
//
#version 460

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec4 inner;
} v;
layout(binding = 1, rg32f) uniform highp image2D arg_0;
vec4 textureLoad_b4d6c4() {
  vec4 res = imageLoad(arg_0, ivec2(min(uvec2(1u), (uvec2(imageSize(arg_0)) - uvec2(1u)))));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_b4d6c4();
}
