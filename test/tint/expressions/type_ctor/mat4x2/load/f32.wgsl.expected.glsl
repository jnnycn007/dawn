#version 310 es

layout(binding = 0, std430)
buffer out_block_1_ssbo {
  mat4x2 inner;
} v;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  mat4x2 m = mat4x2(vec2(0.0f), vec2(0.0f), vec2(0.0f), vec2(0.0f));
  v.inner = mat4x2(m);
}
