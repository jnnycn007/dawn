#version 310 es
#extension GL_AMD_gpu_shader_half_float: require

layout(binding = 0, std140)
uniform u_block_std140_1_ubo {
  f16vec4 inner_col0;
  f16vec4 inner_col1;
  f16vec4 inner_col2;
} v;
f16mat3x4 p = f16mat3x4(f16vec4(0.0hf), f16vec4(0.0hf), f16vec4(0.0hf));
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  p = f16mat3x4(v.inner_col0, v.inner_col1, v.inner_col2);
  p[1u] = f16mat3x4(v.inner_col0, v.inner_col1, v.inner_col2)[0u];
  p[1u] = f16mat3x4(v.inner_col0, v.inner_col1, v.inner_col2)[0u].ywxz;
  p[0u].y = f16mat3x4(v.inner_col0, v.inner_col1, v.inner_col2)[1u].x;
}
